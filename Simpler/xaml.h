// This header becomes "winrt/xaml.h" and shipped with C++/WinRT

#pragma once

namespace winrt::impl
{
    template <typename T>
    struct bind_member
    {
        static Windows::Foundation::IInspectable get(T const& object, hstring const&)
        {
            return box_value(object);
        }

        static void set(T& object, hstring const&, Windows::Foundation::IInspectable const& value)
        {
            object = unbox_value<T>(value);
        }
    };

    template <typename T>
    class has_bind
    {
        template <typename U, typename = decltype(std::declval<U>().bind(L""))> static constexpr bool get_value(int) { return true; }
        template <typename> static constexpr bool get_value(...) { return false; }

    public:

        static constexpr bool value = get_value<T>(0);
    };
}

namespace winrt
{
    // Type returned by winrt::bind function to provide type inference
    struct xaml_binding
    {
        xaml_binding() noexcept {}

        template <typename T>
        xaml_binding(T& value) : m_accessor{ new accessor<T>(value), take_ownership_from_abi }
        {
        }

        template <typename T>
        xaml_binding(T& value, hstring const& name) : m_accessor{ new accessor<T>(value, name), take_ownership_from_abi }
        {
        }

        Windows::Foundation::IInspectable get() const
        {
            if (m_accessor)
            {
                return m_accessor->get();
            }

            return {};
        }

        void set(Windows::Foundation::IInspectable const& value) const
        {
            if (m_accessor)
            {
                m_accessor->set(value);
            }
        }

        bool is_compound() const noexcept
        {
            return m_accessor && m_accessor->is_compound();
        }

        explicit operator bool() const noexcept
        {
            return static_cast<bool>(m_accessor);
        }

    private:

        struct accessor_abi : impl::unknown_abi
        {
            virtual Windows::Foundation::IInspectable get() = 0;
            virtual void set(Windows::Foundation::IInspectable const&) = 0;
            virtual bool is_compound() const noexcept = 0;
        };

        template <typename T>
        struct accessor final : accessor_abi
        {
            accessor(T& value) : m_value(value)
            {
            }

            accessor(T& value, hstring const& name) : m_value(value), m_name(name)
            {
            }

            Windows::Foundation::IInspectable get() final
            {
                if constexpr (impl::has_category_v<T>)
                {
                    return impl::bind_member<T>::get(m_value, m_name);
                }
                else
                {
                    return {};
                }
            }

            void set([[maybe_unused]] Windows::Foundation::IInspectable const& value) final
            {
                if constexpr (impl::has_category_v<T>)
                {
                    impl::bind_member<T>::set(m_value, m_name, value);
                }
            }

            bool is_compound() const noexcept final
            {
                return std::is_compound_v<T>;
            }

            int32_t __stdcall QueryInterface(guid const&, void** result) noexcept final
            {
                *result = nullptr;
                return impl::error_no_interface;
            }

            uint32_t __stdcall AddRef() noexcept final
            {
                return 1 + m_references.fetch_add(1, std::memory_order_relaxed);
            }

            uint32_t __stdcall Release() noexcept final
            {
                uint32_t const target = m_references.fetch_sub(1, std::memory_order_release) - 1;

                if (target == 0)
                {
                    std::atomic_thread_fence(std::memory_order_acquire);
                    delete this;
                }

                return target;
            }

        private:

            std::atomic<uint32_t> m_references{ 1 };
            T& m_value;
            hstring const m_name;
        };

        com_ptr<accessor_abi> m_accessor;
    };

    // In winrt namespace so that developers can overload bind
    template <typename T>
    auto bind(T&& object, hstring const& name)
    {
        if constexpr (impl::has_bind<T>::value)
        {
            return object.bind(name);
        }
        else
        {
            return xaml_binding(object, name);
        }
    }
}

namespace winrt::impl
{
    template <typename T>
    struct bind_object;

    template <typename T>
    struct bind_property : implements<bind_property<T>, Windows::UI::Xaml::Data::ICustomProperty>
    {
        bind_property(com_ptr<bind_object<T>>&& object, T& property, hstring const& name) :
            m_object(std::move(object)),
            m_property(property),
            m_name(name),
            m_binding{ bind(m_property, m_name) }
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
            return {};
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
        com_ptr<bind_object<T>> m_object;
        T m_property;
        hstring m_name;
        xaml_binding m_binding;
    };

    template <typename T>
    struct bind_object : implements<bind_object<T>, Windows::UI::Xaml::Data::ICustomPropertyProvider, Windows::UI::Xaml::Data::INotifyPropertyChanged>
    {
        bind_object(T const& object) :
            m_object(object)
        {
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

        Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& member)
        {
            return make<bind_property<T>>(get_strong(), m_object, member);
        }

        Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const&, Windows::UI::Xaml::Interop::TypeName const&) const noexcept
        {
            return {};
        }

        hstring GetStringRepresentation() const noexcept
        {
            return {};
        }

        Windows::UI::Xaml::Interop::TypeName Type() const noexcept
        {
            return {};
        }

    private:
        T m_object;
        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };

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
            // TODO: Try to avoid magic static (hopefully std::map has constexpr zero-init constructor)
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

        struct xaml_member : implements<xaml_member, Windows::UI::Xaml::Markup::IXamlMember>
        {
            xaml_member(com_ptr<D>&& object, hstring const& name) :
                m_object(std::move(object)),
                m_name(name),
                m_binding{ bind(*m_object, m_name) }
            {
            }

            Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const&)
            {
                if (!m_binding.is_compound() || m_value == nullptr)
                {
                    m_value = m_binding.get();
                }

                return m_value;
            }

            void SetValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const& value)
            {
                m_value = nullptr;
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

            Windows::UI::Xaml::Markup::IXamlType Type() const noexcept
            {
                return {};
            }

            Windows::UI::Xaml::Markup::IXamlType TargetType() const noexcept
            {
                return {};
            }

        private:

            com_ptr<D> m_object;
            hstring const m_name;
            xaml_binding m_binding;
            Windows::Foundation::IInspectable m_value;
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

    // -----------------

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

    // -----------------

    template <typename D, typename... I>
    using xaml_page = xaml_type<D, false, Windows::UI::Xaml::Controls::PageT, I...>;

    template <typename D, typename... I>
    using xaml_user_control = xaml_type<D, true, Windows::UI::Xaml::Controls::UserControlT, I...>;
}
