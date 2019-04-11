// This header becomes "winrt/xaml.h" and shipped with C++/WinRT

#pragma once

namespace winrt
{
    using inspectable = Windows::Foundation::IInspectable;

    template <typename T, typename Allocator = std::allocator<T>>
    Windows::Foundation::Collections::IObservableVector<T> observable_vector(std::vector<T, Allocator> && values = {})
    {
        return single_threaded_observable_vector<T, Allocator>(std::forward<std::vector<T, Allocator>>(values));
    }

    struct xaml_member
    {
        xaml_member() noexcept {}

        template <typename T>
        xaml_member(T& value) : m_accessor(new accessor<T>(value))
        {
        }

        inspectable get() const
        {
            if (m_accessor)
            {
                return m_accessor->get();
            }

            return {};
        }

        void put(inspectable const& value) const
        {
            if (m_accessor)
            {
                m_accessor->put(value);
            }
        }

    private:

        struct accessor_abi
        {
            virtual inspectable get() = 0;
            virtual void put(inspectable const&) = 0;
        };

        template <typename T>
        struct accessor final : accessor_abi
        {
            accessor(T& value) : m_value(value)
            {
            }

            inspectable get() final
            {
                return box_value(m_value);
            }

            void put(inspectable const& value) final
            {
                m_value = unbox_value<T>(value);
            }

        private:

            T& m_value;
        };

        std::unique_ptr<accessor_abi> m_accessor;
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

        std::map<hstring, std::function<Windows::UI::Xaml::Markup::IXamlType()>> m_registry;

        xaml_registry() = default;

        inline static xaml_registry& registry() noexcept
        {
            static xaml_registry s_registry;
            return s_registry;
        }

        void add_type(hstring const& name, std::function<Windows::UI::Xaml::Markup::IXamlType()> const& get)
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

        xaml_type(hstring const& uri)
        {
            Windows::UI::Xaml::Application::LoadComponent(*this, Windows::Foundation::Uri(uri));
        }

        void property_changed(hstring const& name)
        {
            m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
        }

    private:

        struct xaml_member_instance : implements<xaml_member_instance, Windows::UI::Xaml::Markup::IXamlMember>
        {
            xaml_member_instance(hstring const& name) :
                m_name(name)
            {
            }

            hstring Name() const
            {
                return m_name;
            }

            Windows::UI::Xaml::Markup::IXamlType Type() const
            {
                return nullptr;
            }

            inspectable GetValue(inspectable const& instance)
            {
                resolve(instance);
                return m_member.get();
            }

            void SetValue(inspectable const& instance, inspectable const& value)
            {
                resolve(instance);
                m_member.put(value);
                m_instance->property_changed(m_name);
            }

            bool IsReadOnly() const
            {
                return false;
            }

            bool IsAttachable() const noexcept { return {}; }
            bool IsDependencyProperty() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType TargetType() const noexcept { return {}; }

        private:

            void resolve(inspectable const& instance)
            {
                if (m_key != get_abi(instance))
                {
                    m_key = get_abi(instance);
                    m_instance = get_self<D>(instance.as<Windows::UI::Xaml::Data::INotifyPropertyChanged>())->get_strong();
                    m_member = m_instance->bind(m_name);
                }
            }

            hstring const m_name;
            void* m_key{};
            com_ptr<D> m_instance;
            xaml_member m_member;
        };

        struct xaml_type_instance : implements<xaml_type_instance, Windows::UI::Xaml::Markup::IXamlType>
        {
            hstring FullName() const
            {
                return D::GetRuntimeClassName();
            }

            auto ActivateInstance() const
            {
                return make<D>();
            }

            auto BaseType() const
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
                return make<xaml_member_instance>(name);
            }

            Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept { return {}; }
            bool IsArray() const noexcept { return {}; }
            bool IsCollection() const noexcept { return {}; }
            bool IsDictionary() const noexcept { return {}; }
            bool IsMarkupExtension() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept { return {}; }
            inspectable CreateFromString(hstring const&) const noexcept { return {}; }
            void AddToVector(inspectable const&, inspectable const&) const noexcept { }
            void AddToMap(inspectable const&, inspectable const&, inspectable const&) const noexcept { }
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

    struct boxed_value : Windows::Foundation::IInspectable
    {
        boxed_value(std::nullptr_t = nullptr) noexcept {}
        boxed_value(void* ptr, take_ownership_from_abi_t) noexcept : Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}

        template <typename T, typename = std::enable_if_t<impl::has_category_v<std::decay_t<T>>>>
        boxed_value(T && value) : Windows::Foundation::IInspectable(box_value(std::forward<T>(value)))
        {
        }

        template <typename T, typename = std::enable_if_t<impl::has_category_v<std::decay_t<T>>>>
        operator T() const
        {
            return unbox_value<T>(*this);
        }
    };
}

namespace winrt::impl
{
    template <> struct guid_storage<boxed_value>
    {
        static constexpr guid value{ 0xAF86E2E0,0xB12D,0x4C6A,{ 0x9C,0x5A,0xD7,0xAA,0x65,0x10,0x1E,0x90 } };
    };

    template <> struct category<boxed_value>
    {
        using type = basic_category;
    };

    template <> struct name<boxed_value>
    {
        static constexpr auto& value{ L"Object" };
        static constexpr auto& data{ "cinterface(IInspectable)" };
    };
}
