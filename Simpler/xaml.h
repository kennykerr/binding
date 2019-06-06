// This header becomes "winrt/xaml.h" and shipped with C++/WinRT

#pragma once

namespace winrt::impl
{
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
    template <typename T, std::enable_if_t<impl::has_bind<T>::value, int> = 0>
    auto bind(T&& object, hstring const& name)
    {
        return object.bind(name);
    }

    struct xaml_member
    {
        xaml_member() noexcept {}

        template <typename T>
        xaml_member(T& value) : m_accessor{ new accessor<T>(value), take_ownership_from_abi }
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

        void put(Windows::Foundation::IInspectable const& value) const
        {
            if (m_accessor)
            {
                m_accessor->put(value);
            }
        }

        xaml_member bind(hstring const& name) const
        {
            if (m_accessor)
            {
                return m_accessor->bind(name);
            }

            return {};
        }

        bool can_bind() const noexcept
        {
            if (m_accessor)
            {
                return m_accessor->can_bind();
            }

            return false;
        }

        explicit operator bool() const noexcept
        {
            return static_cast<bool>(m_accessor);
        }

    private:

        struct accessor_abi : impl::unknown_abi
        {
            virtual Windows::Foundation::IInspectable get() = 0;
            virtual void put(Windows::Foundation::IInspectable const&) = 0;
            virtual xaml_member bind(hstring const& name) = 0;
            virtual bool can_bind() noexcept = 0;
        };

        template <typename T>
        struct accessor final : accessor_abi
        {
            accessor(T& value) : m_value(value)
            {
            }

            Windows::Foundation::IInspectable get() final
            {
                if constexpr (impl::has_category_v<T>)
                {
                    return winrt::box_value(m_value);
                }
                else
                {
                    return {};
                }
            }

            void put([[maybe_unused]] Windows::Foundation::IInspectable const& value) final
            {
                if constexpr (impl::has_category_v<T>)
                {
                    m_value = unbox_value<T>(value);
                }
            }

            xaml_member bind([[maybe_unused]] hstring const& name) final
            {
                if constexpr (std::is_compound_v<T>)
                {
                    return winrt::bind(m_value, name);
                }
                else
                {
                    return {};
                }
            }

            bool can_bind() noexcept final
            {
                return std::is_class_v<T>;
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
        };

        com_ptr<accessor_abi> m_accessor;
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

        static bool add_once(Windows::UI::Xaml::Markup::IXamlType const& type)
        {
            registry().add_type(type.FullName(), [type]
                {
                    return type;
                });

            return true;
        }

        static Windows::UI::Xaml::Markup::IXamlType get(hstring const& name)
        {
            return registry().get_type(name);
        }

    private:

        std::map<hstring, delegate<Windows::UI::Xaml::Markup::IXamlType()>> m_registry;

        xaml_registry() = default;

        inline static xaml_registry& registry() noexcept
        {
            static xaml_registry s_registry;
            return s_registry;
        }

        void add_type(hstring const& name, delegate<Windows::UI::Xaml::Markup::IXamlType()> const& get)
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

    template <typename D, bool Register = true>
    struct xaml_registration
    {
        static inline bool registered{ xaml_registry::add<D>() };
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
            return xaml_registry::get(name);
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

        struct xaml_member_instance : implements<xaml_member_instance, Windows::UI::Xaml::Markup::IXamlMember>//, Windows::UI::Xaml::Data::ICustomPropertyProvider>
        {

            Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
            {
                name;
                return nullptr;
            }
            Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type) const
            {
                name;
                type;
                return nullptr;
            }
            hstring GetStringRepresentation() const
            {
                return L"GetStringRepresentation";
            }
            auto Type()
            {
                struct result
                {
                    xaml_member_instance* instance;

                    // TODO: make these call member functions

                    operator Windows::UI::Xaml::Markup::IXamlType()
                    {
                        //instance->resolve();

                        //if (auto member = instance->m_member.can_bind())
                        //{
                        //    return make<member_type>(*instance, instance->m_member, instance->m_name);
                        //}

                        return nullptr;
                    }

                    operator Windows::UI::Xaml::Interop::TypeName()
                    {
                        return { L"Name", Windows::UI::Xaml::Interop::TypeKind::Custom };
                    }
                };

                return result{ this };
            }

            xaml_member_instance(D* instance, hstring const& name) :
                m_name(name)
            {
                m_instance.copy_from(instance);
            }

            hstring Name() const
            {
                return m_name;
            }

            Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const& instance)
            {
                WINRT_ASSERT(instance == *m_instance);
                resolve();
                return m_member.get();
            }

            void SetValue(Windows::Foundation::IInspectable const& instance, Windows::Foundation::IInspectable const& value)
            {
                WINRT_ASSERT(instance == *m_instance);
                resolve();
                m_member.put(value);
                m_instance->property_changed(m_name);
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

            Windows::UI::Xaml::Markup::IXamlType TargetType() const noexcept
            {
                return {};
            }

        private:

            //struct member_type : implements<member_type, Windows::UI::Xaml::Markup::IXamlType, Windows::UI::Xaml::Data::ICustomPropertyProvider>
            //{
            //    Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
            //    {
            //        name;
            //        return nullptr;
            //    }
            //    Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type) const
            //    {
            //        name;
            //        type;
            //        return nullptr;
            //    }
            //    hstring GetStringRepresentation() const
            //    {
            //        return L"GetStringRepresentation";
            //    }
            //    Windows::UI::Xaml::Interop::TypeName Type()
            //    {
            //        return { L"Name", Windows::UI::Xaml::Interop::TypeKind::Custom };
            //    }

            //    member_type(Windows::UI::Xaml::Markup::IXamlMember const& xamlMember, xaml_member const& member, hstring const& name) :
            //        m_xamlMember(xamlMember),
            //        m_member(member),
            //        m_name(name)
            //    {
            //    }

            //    hstring FullName() const
            //    {
            //        return L"Custom." + m_name;
            //    }

            //    Windows::Foundation::IInspectable ActivateInstance() const
            //    {
            //        return nullptr;
            //    }

            //    Windows::UI::Xaml::Markup::IXamlType BaseType() const
            //    {
            //        return nullptr;
            //    }

            //    bool IsConstructible() const
            //    {
            //        return false;
            //    }

            //    Windows::UI::Xaml::Interop::TypeName UnderlyingType() const
            //    {
            //        xaml_registry::add_once(*this);
            //        return { L"Custom." + m_name, Windows::UI::Xaml::Interop::TypeKind::Primitive };
            //    }

            //    bool IsBindable() const
            //    {
            //        return true;
            //    }

            //    Windows::UI::Xaml::Markup::IXamlMember GetMember(hstring const& name) const
            //    {
            //        name;
            //        return nullptr;
            //    }

            //    Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept
            //    {
            //        return m_xamlMember;
            //    }
            //    bool IsArray() const noexcept
            //    {
            //        return {};
            //    }
            //    bool IsCollection() const noexcept
            //    {
            //        return {};
            //    }
            //    bool IsDictionary() const noexcept
            //    {
            //        return {};
            //    }
            //    bool IsMarkupExtension() const noexcept
            //    {
            //        return {};
            //    }
            //    Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept
            //    {
            //        return {};
            //    }
            //    Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept
            //    {
            //        return {};
            //    }

            //    Windows::Foundation::IInspectable CreateFromString(hstring const& value) const noexcept
            //    {
            //        value;
            //        return {};
            //    }

            //    void AddToVector(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
            //    {
            //    }
            //    void AddToMap(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
            //    {
            //    }
            //    void RunInitializer() const noexcept
            //    {
            //    }

            //private:

            //    Windows::UI::Xaml::Markup::IXamlMember m_xamlMember;
            //    xaml_member m_member;
            //    hstring const m_name;
            //};

            void resolve()
            {
                if (!m_member)
                {
                    m_member = bind(*m_instance, m_name);
                }
            }

            com_ptr<D> m_instance;
            hstring const m_name;
            xaml_member m_member;
        };

        struct xaml_type_instance : implements<xaml_type_instance, Windows::UI::Xaml::Markup::IXamlType, Windows::UI::Xaml::Data::ICustomPropertyProvider>
        {
            Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
            {
                name;
                return nullptr;
            }
            Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type) const
            {
                name;
                type;
                return nullptr;
            }
            hstring GetStringRepresentation() const
            {
                return L"GetStringRepresentation";
            }
            Windows::UI::Xaml::Interop::TypeName Type()
            {
                return { L"Name", Windows::UI::Xaml::Interop::TypeKind::Custom };
            }

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
                return make<xaml_member_instance>(s_last_type, name);
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
    using xaml_page = xaml_type<D, false, Windows::UI::Xaml::Controls::PageT, I...>;

    template <typename D, typename... I>
    using xaml_user_control = xaml_type<D, true, Windows::UI::Xaml::Controls::UserControlT, I...>;
}
