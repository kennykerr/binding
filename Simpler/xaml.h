#pragma once

// TODO: This goes in windows.foundation.h
namespace winrt
{
    template <typename T>
    class has_bind_member
    {
        template <typename U, typename = decltype(std::declval<U>().bind(L""))> static constexpr bool get_value(int) { return true; }
        template <typename> static constexpr bool get_value(...) { return false; }

    public:

        static constexpr bool value = get_value<T>(0);
    };

    template <typename T>
    class has_bind_free
    {
        template <typename U, typename = decltype(winrt::bind(std::declval<U>(), L""))> static constexpr bool get_value(int) { return true; }
        template <typename> static constexpr bool get_value(...) { return false; }

    public:

        static constexpr bool value = get_value<T>(0);
    };

    template <typename T>
    inline constexpr bool can_bind_v = has_bind_member<T>::value || has_bind_free<T>::value;

    template <typename T>
    auto bind_member(T&& object, hstring const& name)
    {
        if constexpr (has_bind_free<T>::value)
        {
            return winrt::bind(object, name);
        }
        else
        {
            return object.bind(name);
        }
    }

    struct binding
    {
        binding() noexcept
        {
        }

        // TODO: add overloads that take leading strong/weak reference
        template <typename T, typename = std::enable_if_t<!std::is_base_of_v<Windows::Foundation::IUnknown, T>>>
        binding(T& reference) : binding
        {
            [&]
            {
                return reference;
            },
            [&](T const& value)
            {
                reference = value;
            }
        }
        {
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Windows::Foundation::IUnknown, T>>>
        binding(T const& object) : binding
        {
            [object]
            {
                return object;
            },
            [](T const&) {}
        }
        {
        }

        template <typename Get, typename = std::enable_if_t<std::is_invocable_v<Get>>>
        binding(Get && get_self) : binding
        {
            std::forward<Get>(get_self),
            [](auto&&) {}
        }
        {
        }

        template <typename Get, typename Set>
        binding(Get&& get_self, Set&& set_self) : m_accessor
        {
            std::make_unique<accessor<Get, Set>>(
                std::forward<Get>(get_self),
                std::forward<Set>(set_self))
        }
        {
        }

        Windows::Foundation::IInspectable get() const
        {
            if (m_accessor)
            {
                return m_accessor->get();
            }

            return nullptr;
        }

        void set(Windows::Foundation::IInspectable const& value) const
        {
            if (m_accessor)
            {
                m_accessor->set(value);
            }
        }

        bool can_bind() const
        {
            if (m_accessor)
            {
                return m_accessor->can_bind();
            }

            return false;
        }

        binding bind(hstring const& name) const
        {
            if (m_accessor)
            {
                return m_accessor->bind(name);
            }

            return {};
        }

    private:

        struct accessor_abi
        {
            virtual Windows::Foundation::IInspectable get() = 0;
            virtual void set(Windows::Foundation::IInspectable const& value) = 0;
            virtual bool can_bind() = 0;
            virtual binding bind(hstring const& name) = 0;
        };

        template <typename Get, typename Set>
        struct accessor final : accessor_abi, Get, Set
        {
            using type = decltype(std::declval<Get>()());

            accessor(Get&& get_self, Set&& set_self) :
                Get(std::forward<Get>(get_self)),
                Set(std::forward<Set>(set_self))
            {
            }

            Windows::Foundation::IInspectable get() final
            {
                return box_value((*this)());
            }

            void set(Windows::Foundation::IInspectable const& value) final
            {
                (*this)(unbox_value<type>(value));
            }

            bool can_bind() final
            {
                return can_bind_v<type>;
            }

            binding bind([[maybe_unused]] hstring const& name) final
            {
                if constexpr (can_bind_v<type>)
                {
                    return bind_member((*this)(), name);
                }

                return {};
            }
        };

        std::unique_ptr<accessor_abi> m_accessor;
    };
}

// TODO: this goes into windows.ui.xaml.h and forward declares the .Data/.Markup stuff?
namespace winrt::impl
{
    struct xaml_registry
    {
        template <typename T>
        static bool add()
        {
            registry().add_type(T::GetRuntimeClassName(), []
                {
                    static auto type = T::get_type();
                    return type;
                });

            return true;
        }

        static Windows::UI::Xaml::Markup::IXamlType get(hstring const& name)
        {
            return registry().get_type(name);
        }

    private:

        std::map<hstring, winrt::delegate<Windows::UI::Xaml::Markup::IXamlType()>> m_registry;

        xaml_registry() = default;

        inline static xaml_registry& registry() noexcept
        {
            // TODO: Try to avoid magic static
            static xaml_registry s_registry;
            return s_registry;
        }

        void add_type(hstring const& name, winrt::delegate<Windows::UI::Xaml::Markup::IXamlType()> const& get)
        {
            if (m_registry.find(name) == m_registry.end())
            {
                m_registry[name] = get;
            }
        }

        Windows::UI::Xaml::Markup::IXamlType get_type(hstring const& name) const
        {
            auto info = m_registry.find(name);

            if (info != m_registry.end())
            {
                return info->second();
            }

            return {};
        }
    };
}

namespace winrt
{
    template <typename D, bool Register = true>
    struct xaml_registration
    {
        static inline bool registered{ impl::xaml_registry::add<D>() };
    };

    template <typename D>
    struct xaml_registration<D, false>
    {
    };

    template <typename D, typename... I>
    struct xaml_app : Windows::UI::Xaml::ApplicationT<D, Windows::UI::Xaml::Markup::IXamlMetadataProvider, I...>
    {
        Windows::UI::Xaml::Markup::IXamlType GetXamlType(Windows::UI::Xaml::Interop::TypeName const& type) const
        {
            return GetXamlType(type.Name);
        }

        Windows::UI::Xaml::Markup::IXamlType GetXamlType(hstring const& name) const
        {
            return impl::xaml_registry::get(name);
        }

        com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions() const
        {
            return {};
        }
    };

    template <typename D, bool R, template <typename...> typename B, typename... I>
    struct xaml_type : B<D, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>, xaml_registration<D, R>
    {
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_changed.add(handler);
        }

        void PropertyChanged(winrt::event_token token)
        {
            m_changed.remove(token);
        }

        static Windows::UI::Xaml::Markup::IXamlType get_type()
        {
            return make<xaml_type_instance>();
        }

        static hstring GetRuntimeClassName()
        {
            return D::type_name();
        }

    protected:

        using base_type = xaml_type<D, R, B, I...>;

        // TODO: weak/strong reference?
        static inline D* s_last_type;

        xaml_type(hstring const& uri)
        {
            Windows::UI::Xaml::Application::LoadComponent(*this, Windows::Foundation::Uri(uri));
            s_last_type = static_cast<D*>(this);
        }

        void property_changed(hstring const& name)
        {
            m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
        }

    private:

        struct xaml_member;

        struct bind_property : implements<bind_property, Windows::UI::Xaml::Data::ICustomProperty>
        {
            bind_property(com_ptr<xaml_member> && object, binding&& binding, hstring const& name) :
                m_object(std::move(object)),
                m_binding(std::move(binding)),
                m_name(name)
            {
            }

            auto GetValue(Windows::Foundation::IInspectable const&) const
            {
                return m_binding.get();
            }

            void SetValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const& value)
            {
                m_binding.set(value);
                m_object->property_changed(m_name);
            }

            bool CanWrite() const noexcept
            {
                return true;
            }

            bool CanRead() const noexcept
            {
                return true;
            }

            Windows::UI::Xaml::Interop::TypeName Type() const noexcept
            {
                // TODO: xaml_typename<T> from the binding
                return{};
            }

            hstring Name() const noexcept
            {
                return {};
            }

            Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
            {
                return {};
            }

            void SetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
            {
            }

        private:
            com_ptr<xaml_member> m_object;
            binding m_binding;
            hstring m_name;
        };

        struct xaml_member : implements<xaml_member, Windows::UI::Xaml::Markup::IXamlMember, Windows::UI::Xaml::Data::ICustomPropertyProvider, Windows::UI::Xaml::Data::INotifyPropertyChanged>
        {
            xaml_member(com_ptr<D>&& object, hstring const& name) :
                m_object(std::move(object)),
                m_name(name),
                m_binding{ bind_member(*m_object, m_name) }
            {
            }

            Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const&)
            {
                if (m_binding.can_bind())
                {
                    return *this;
                }
                else
                {
                    return m_binding.get();
                }
            }

            void SetValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const& value)
            {
                m_binding.set(value);
                m_object->property_changed(m_name);
            }

            bool IsReadOnly() const
            {
                return false;
            }

            bool IsAttachable() const noexcept
            {
                return {};
            }

            bool IsDependencyProperty() const noexcept
            {
                return {};
            }

            hstring Name() const noexcept
            {
                return {};
            }

            auto Type() const noexcept
            {
                struct result
                {
                    operator Windows::UI::Xaml::Markup::IXamlType()
                    {
                        return {};
                    }
                    operator Windows::UI::Xaml::Interop::TypeName()
                    {
                        return {};
                    }
                };

                return result{};
            }

            Windows::UI::Xaml::Markup::IXamlType TargetType() const noexcept
            {
                return {};
            }

            Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& member)
            {
                return make<bind_property>(get_strong(), m_binding.bind(member), member);
            }

            Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const&, Windows::UI::Xaml::Interop::TypeName const&) const noexcept
            {
                return {};
            }

            hstring GetStringRepresentation() const noexcept
            {
                return {};
            }

            void property_changed(hstring const& name)
            {
                m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
            }

            event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
            {
                return m_changed.add(handler);
            }

            void PropertyChanged(winrt::event_token const& token)
            {
                m_changed.remove(token);
            }

        private:

            com_ptr<D> m_object;
            hstring const m_name;
            binding m_binding;
            event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
        };

        struct xaml_type_instance : implements<xaml_type_instance, Windows::UI::Xaml::Markup::IXamlType>
        {
            hstring FullName() const
            {
                return D::GetRuntimeClassName();
            }

            Windows::Foundation::IInspectable ActivateInstance() const
            {
                return make<D>();
            }

            Windows::UI::Xaml::Markup::IXamlType BaseType() const
            {
                return nullptr;
            }

            bool IsConstructible() const
            {
                return true;
            }

            Windows::UI::Xaml::Interop::TypeName UnderlyingType() const
            {
                return {};
            }

            bool IsBindable() const
            {
                return true;
            }

            Windows::UI::Xaml::Markup::IXamlMember GetMember(hstring const& name) const
            {
                static std::map<hstring, Windows::UI::Xaml::Markup::IXamlMember> members;

                auto found = members.find(name);

                if (found != members.end())
                {
                    return found->second;
                }

                return members[name] = make<xaml_member>(s_last_type->get_strong(), name);
            }

            Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept { return {}; }
            bool IsArray() const noexcept { return {}; }
            bool IsCollection() const noexcept { return {}; }
            bool IsDictionary() const noexcept { return {}; }
            bool IsMarkupExtension() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept { return {}; }

            Windows::Foundation::IInspectable CreateFromString(hstring const& value) const noexcept
            {
                value;
                return {};
            }

            void AddToVector(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept { }
            void AddToMap(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept { }
            void RunInitializer() const noexcept { }
        };

        static hstring type_name()
        {
            return {};
        }

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };

    template <typename D, typename... I>
    struct registered_implements : implements<D, I...>, xaml_registration<D, true>
    {
        static Windows::UI::Xaml::Markup::IXamlType get_type()
        {
            return make<xaml_type_instance>();
        }

        static hstring GetRuntimeClassName()
        {
            return D::type_name();
        }

    private:

        struct xaml_type_instance : implements<xaml_type_instance, Windows::UI::Xaml::Markup::IXamlType>
        {
            hstring FullName() const
            {
                return D::GetRuntimeClassName();
            }

            Windows::Foundation::IInspectable ActivateInstance() const
            {
                return make<D>();
            }

            Windows::UI::Xaml::Markup::IXamlType BaseType() const
            {
                return nullptr;
            }

            bool IsConstructible() const
            {
                return true;
            }

            Windows::UI::Xaml::Interop::TypeName UnderlyingType() const
            {
                return {};
            }

            bool IsBindable() const
            {
                return true;
            }

            Windows::UI::Xaml::Markup::IXamlMember GetMember(hstring const& name) const
            {
                name;
                return nullptr;
            }

            Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept { return {}; }
            bool IsArray() const noexcept { return {}; }
            bool IsCollection() const noexcept { return {}; }
            bool IsDictionary() const noexcept { return {}; }
            bool IsMarkupExtension() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept { return {}; }

            Windows::Foundation::IInspectable CreateFromString(hstring const& value) const noexcept
            {
                value;
                return {};
            }

            void AddToVector(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept { }
            void AddToMap(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept { }
            void RunInitializer() const noexcept { }
        };
    };

    template <typename D, typename... I>
    using xaml_page = xaml_type<D, false, Windows::UI::Xaml::Controls::PageT, I...>;

    template <typename D, typename... I>
    using xaml_user_control = xaml_type<D, true, Windows::UI::Xaml::Controls::UserControlT, I...>;
}
