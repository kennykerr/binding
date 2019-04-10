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

    template<typename D, bool register_type = true>
    struct xaml_registration
    {
        static inline bool registered{ xaml_registry::add<D>() };
    };

    template<typename D>
    struct xaml_registration<D, false>
    {
    };

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

    template<typename T>
    struct xaml_non_observable_type : xaml_registration<T>
    {
        static void property_changed(T* /*self*/, hstring const& /*name*/) {}

        static Windows::UI::Xaml::Markup::IXamlType get_type()
        {
            return xaml_type_base<T>::get_type();
        }
    };

    template<typename T>
    struct xaml_traits
    {
        static hstring GetRuntimeClassName()
        {
            return T::GetRuntimeClassName();
        }

        static xaml_member bind(T* self, hstring const& name)
        {
            return self->bind(name);
        }

        static void property_changed(T* self, hstring const& name)
        {
            self->property_changed(name);
        }

        static auto get_type()
        {
            return T::get_type();
        }
    };

    struct xaml_registry
    {
        template <typename T>
        static bool add()
        {
            registry().add_type(xaml_traits<T>::GetRuntimeClassName(), []
                {
                    static auto type = xaml_traits<T>::get_type();
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

    template <typename D, bool register_type = true>
    struct xaml_type_base : xaml_registration<D, register_type>
    {
        static Windows::UI::Xaml::Markup::IXamlType get_type()
        {
            return make<xaml_type_instance>();
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

            inspectable GetValue(inspectable const& instance) const
            {
                return xaml_traits<D>::bind(get_self<D>(instance), m_name).get();
            }

            void SetValue(inspectable const& instance, inspectable const& value) const
            {
                auto self = get_self<D>(instance);
                xaml_traits<D>::bind(self, m_name).put(value);
                xaml_traits<D>::property_changed(self, m_name);
            }

            bool IsReadOnly() const
            {
                return false;
            }

            bool IsAttachable() const noexcept { return {}; }
            bool IsDependencyProperty() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType TargetType() const noexcept { return {}; }

        private:

            hstring m_name;
        };

        struct xaml_type_instance : implements<xaml_type_instance, Windows::UI::Xaml::Markup::IXamlType>
        {
            hstring FullName() const
            {
                return xaml_traits<D>::GetRuntimeClassName();
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
    };

    template <typename D, bool register_type, template <typename...> typename B, typename... I>
    struct xaml_observable : B<D, inspectable, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>, xaml_type_base<D, register_type>
    {
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_changed.add(handler);
        }

        void PropertyChanged(winrt::event_token token)
        {
            m_changed.remove(token);
        }

    protected:

        friend struct xaml_traits<D>;

        using base_type = xaml_observable<D, register_type, B, I...>;

        xaml_observable(hstring const& uri)
        {
            DataContext(*this);
            Windows::UI::Xaml::Application::LoadComponent(*this, Windows::Foundation::Uri(uri));
        }

        void property_changed(hstring const& name)
        {
            m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
        }

    private:

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };

    template <typename D, typename... I>
    using xaml_page = xaml_observable<D, false, Windows::UI::Xaml::Controls::PageT, I...>;

    template <typename D, typename... I>
    using xaml_user_control = xaml_observable<D, true, Windows::UI::Xaml::Controls::UserControlT, I...>;
}
