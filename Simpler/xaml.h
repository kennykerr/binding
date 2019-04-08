// This header becomes "winrt/xaml.h" and shipped with C++/WinRT

#pragma once

namespace winrt
{
    struct xaml_member_info
    {
        Windows::UI::Xaml::Markup::IXamlType type;
        std::function<Windows::Foundation::IInspectable(Windows::Foundation::IInspectable)> get;
        std::function<void(Windows::Foundation::IInspectable, Windows::Foundation::IInspectable)> set;
    };

    struct xaml_type_info
    {
        hstring name;
        std::function<Windows::Foundation::IInspectable()> create;
        Windows::UI::Xaml::Markup::IXamlType base;
        std::function<xaml_member_info(hstring const&)> member;
    };

    struct xaml_registry
    {
        template <typename T>
        static bool add()
        {
            return registry().add_type(
            {
                T::GetRuntimeClassName(),
                [] { return make<T>(); },
                get(L"Object"),
                [](hstring const& memberName) { return T::get_member(memberName); }
            });
        }

        static Windows::UI::Xaml::Markup::IXamlType get(hstring const& typeName)
        {
            return registry().get_type(typeName);
        }

    private:

        struct xaml_type : implements<xaml_type, Windows::UI::Xaml::Markup::IXamlType>
        {
            xaml_type(xaml_type_info const& info) :
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
                struct xaml_member : implements<xaml_member, Windows::UI::Xaml::Markup::IXamlMember>
                {
                    xaml_member(hstring const& name, xaml_member_info&& info) :
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

                    Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const& instance) const
                    {
                        return m_info.get(instance);
                    }

                    void SetValue(Windows::Foundation::IInspectable const& instance, Windows::Foundation::IInspectable const& value) const
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

                return make<xaml_member>(name, m_info.member(name));
            }

            Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept { return {}; }
            bool IsArray() const noexcept { return {}; }
            bool IsCollection() const noexcept { return {}; }
            bool IsDictionary() const noexcept { return {}; }
            bool IsMarkupExtension() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept { return {}; }
            IInspectable CreateFromString(hstring const& /*value*/) const noexcept { return {}; }
            void AddToVector(Windows::Foundation::IInspectable const& /*instance*/, Windows::Foundation::IInspectable const& /*value*/) const noexcept { }
            void AddToMap(Windows::Foundation::IInspectable const& /*instance*/, Windows::Foundation::IInspectable const& /*key*/, Windows::Foundation::IInspectable const& /*value*/) const noexcept { }
            void RunInitializer() const noexcept { }

        private:

            xaml_type_info m_info;
        };

        // TODO : can combine into a single collection
        std::map<hstring, xaml_type_info> m_registration;
        std::map<hstring, Windows::UI::Xaml::Markup::IXamlType> m_typeCache;

        xaml_registry() = default;

        inline static xaml_registry& registry() noexcept
        {
            static xaml_registry s_registry;
            return s_registry;
        }

        bool add_type(xaml_type_info const& registration)
        {
            auto typeName = registration.name;
            if (m_registration.find(typeName) == m_registration.end())
            {
                m_registration[typeName] = registration;
                return true;
            }

            return false;
        }

        Windows::UI::Xaml::Markup::IXamlType get_type(hstring const& typeName)
        {
            auto typeEntry = m_typeCache.find(typeName);
            if (typeEntry != m_typeCache.end())
            {
                return typeEntry->second;
            }

            auto registryEntry = m_registration.find(typeName);
            if (registryEntry != m_registration.end())
            {
                auto xamlType = make<xaml_type>(registryEntry->second);
                m_typeCache[typeName] = xamlType;
                return xamlType;
            }

            return nullptr;
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
