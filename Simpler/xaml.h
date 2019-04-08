// This header becomes "winrt/xaml.h" and shipped with C++/WinRT

#pragma once

namespace winrt
{
    using inspectable = Windows::Foundation::IInspectable;

    struct xaml_member_info
    {
        Windows::UI::Xaml::Markup::IXamlType type;
        std::function<inspectable(inspectable)> get;
        std::function<void(inspectable, inspectable)> set;
    };

    struct xaml_type_info
    {
        hstring name;
        std::function<inspectable()> create;
        Windows::UI::Xaml::Markup::IXamlType base;
        std::function<xaml_member_info(hstring const&)> member;
        Windows::UI::Xaml::Markup::IXamlType type;
    };

    struct xaml_member_instance : implements<xaml_member_instance, Windows::UI::Xaml::Markup::IXamlMember>
    {
        xaml_member_instance(hstring const& name, xaml_member_info&& info) :
            m_name(name),
            m_info(std::move(info))
        {
        }

        hstring Name() const
        {
            return m_name;
        }

        Windows::UI::Xaml::Markup::IXamlType Type() const
        {
            return m_info.type;
        }

        inspectable GetValue(inspectable const& instance) const
        {
            return m_info.get(instance);
        }

        void SetValue(inspectable const& instance, inspectable const& value) const
        {
            m_info.set(instance, value);
        }

        bool IsReadOnly() const
        {
            return m_info.set == nullptr;
        }

        bool IsAttachable() const noexcept { return {}; }
        bool IsDependencyProperty() const noexcept { return {}; }
        Windows::UI::Xaml::Markup::IXamlType TargetType() const noexcept { return {}; }

    private:

        hstring m_name;
        xaml_member_info m_info;
    };

    struct xaml_type_instance : implements<xaml_type_instance, Windows::UI::Xaml::Markup::IXamlType>
    {
        xaml_type_instance(xaml_type_info const& info) :
            m_info(info)
        {
        }

        hstring FullName() const
        {
            return m_info.name;
        }

        auto ActivateInstance() const
        {
            return m_info.create();
        }

        auto BaseType() const
        {
            return m_info.base;
        }

        bool IsConstructible() const
        {
            return m_info.create != nullptr;
        }

        Windows::UI::Xaml::Interop::TypeName UnderlyingType() const
        {
            return
            {
                m_info.name,
                m_info.base ? Windows::UI::Xaml::Interop::TypeKind::Custom : Windows::UI::Xaml::Interop::TypeKind::Primitive
            };
        }

        bool IsBindable() const
        {
            return true;
        }

        Windows::UI::Xaml::Markup::IXamlMember GetMember(hstring const& name) const
        {
            return make<xaml_member_instance>(name, m_info.member(name));
        }

        Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept { return {}; }
        bool IsArray() const noexcept { return {}; }
        bool IsCollection() const noexcept { return {}; }
        bool IsDictionary() const noexcept { return {}; }
        bool IsMarkupExtension() const noexcept { return {}; }
        Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept { return {}; }
        Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept { return {}; }
        IInspectable CreateFromString(hstring const&) const noexcept { return {}; }
        void AddToVector(inspectable const&, inspectable const&) const noexcept { }
        void AddToMap(inspectable const&, inspectable const&, inspectable const&) const noexcept { }
        void RunInitializer() const noexcept { }

    private:

        xaml_type_info m_info;
    };

    struct xaml_registry
    {
        template <typename T>
        static bool add()
        {
            registry().add_type(
            {
                T::GetRuntimeClassName(),
                [] { return make<T>(); },
                get(L"Object"),
                [](hstring const& name) { return T::get_member(name); }
            });

            return true;
        }

        static Windows::UI::Xaml::Markup::IXamlType get(hstring const& typeName)
        {
            return registry().get_type(typeName);
        }

    private:

        std::map<hstring, xaml_type_info> m_registry;

        xaml_registry() = default;

        inline static xaml_registry& registry() noexcept
        {
            static xaml_registry s_registry;
            return s_registry;
        }

        void add_type(xaml_type_info const& info)
        {
            if (m_registry.find(info.name) == m_registry.end())
            {
                m_registry[info.name] = info;
            }
        }

        Windows::UI::Xaml::Markup::IXamlType get_type(hstring const& name)
        {
            auto info = m_registry.find(name);

            if (info == m_registry.end())
            {
                return nullptr;
            }

            if (!info->second.type)
            {
                info->second.type = make<xaml_type_instance>(info->second);
            }

            return info->second.type;
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

    template <typename D, template <typename...> typename B, typename... I>
    struct xaml_type : B<D, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
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

        using base_type = xaml_type<D, B, I...>;

        xaml_type(hstring const& uri)
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
    using xaml_page = xaml_type<D, Windows::UI::Xaml::Controls::PageT, I...>;

    template <typename D, typename... I>
    using xaml_user_control = xaml_type<D, Windows::UI::Xaml::Controls::UserControlT, I...>;
}
