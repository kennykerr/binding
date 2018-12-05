#pragma once

namespace winrt
{
    struct xaml_binding_member : implements<xaml_binding_member, Windows::UI::Xaml::Markup::IXamlMember>
    {
        bool IsAttachable() const
        {
            return {};
        }

        bool IsDependencyProperty() const
        {
            return {};
        }

        bool IsReadOnly() const
        {
            return {};
        }

        hstring Name() const
        {
            return {};
        }

        Windows::UI::Xaml::Markup::IXamlType TargetType() const
        {
            return {};
        }

        Windows::UI::Xaml::Markup::IXamlType Type() const
        {
            return {};
        }

        Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const& instance) const
        {
            return box_value(L"Value");
        }

        void SetValue(Windows::Foundation::IInspectable const& instance, Windows::Foundation::IInspectable const& value) const
        {
        }
    };

    struct xaml_binding_type : implements<xaml_binding_type, Windows::UI::Xaml::Markup::IXamlType>
    {
        Windows::UI::Xaml::Markup::IXamlType BaseType() const
        {
            return {};
        }

        Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const
        {
            return {};
        }

        hstring FullName() const
        {
            return {};
        }

        bool IsArray() const
        {
            return {};
        }

        bool IsCollection() const
        {
            return {};
        }

        bool IsConstructible() const
        {
            return {};
        }

        bool IsDictionary() const
        {
            return {};
        }

        bool IsMarkupExtension() const
        {
            return {};
        }

        bool IsBindable() const
        {
            return true;
        }

        Windows::UI::Xaml::Markup::IXamlType ItemType() const
        {
            return {};
        }

        Windows::UI::Xaml::Markup::IXamlType KeyType() const
        {
            return {};
        }

        Windows::UI::Xaml::Interop::TypeName UnderlyingType() const
        {
            return {};
        }

        Windows::Foundation::IInspectable ActivateInstance() const
        {
            return {};
        }

        Windows::Foundation::IInspectable CreateFromString(hstring const& value) const
        {
            return {};
        }

        Windows::UI::Xaml::Markup::IXamlMember GetMember(hstring const& name) const
        {
            if (name == L"DisplayText")
            {
                return make< xaml_binding_member>();
            }

            return nullptr;
        }

        void AddToVector(Windows::Foundation::IInspectable const& instance, Windows::Foundation::IInspectable const& value) const
        {
        }

        void AddToMap(Windows::Foundation::IInspectable const& instance, Windows::Foundation::IInspectable const& key, Windows::Foundation::IInspectable const& value) const
        {
        }

        void RunInitializer() const
        {
        }
    };

    template <typename D, typename... I>
    struct xaml_binding_app : Windows::UI::Xaml::ApplicationT<D, Windows::UI::Xaml::Markup::IXamlMetadataProvider, I...>
    {
        Windows::UI::Xaml::Markup::IXamlType GetXamlType(Windows::UI::Xaml::Interop::TypeName const& type) const
        {
            return {};
        }

        Windows::UI::Xaml::Markup::IXamlType GetXamlType(hstring const& fullName) const
        {
            if (fullName == L"MainPage")
            {
                return make<xaml_binding_type>();
            }

            return nullptr;
        }

        com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions() const
        {
            return {};
        }
    };

    template <typename D, template <typename...> typename B, typename... I>
    struct xaml_binding : B<D, Windows::UI::Xaml::Data::ICustomPropertyProvider, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
    {
        xaml_binding()
        {
            // TODO: circular reference?
            this->DataContext(*this);
        }

        Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name)
        {
            return static_cast<D*>(this)->bind(name);
        }

        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_changed.add(handler);
        }

        void PropertyChanged(winrt::event_token token)
        {
            m_changed.remove(token);
        }

        Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const&, Windows::UI::Xaml::Interop::TypeName const&) const noexcept { return {}; }
        hstring GetStringRepresentation() const noexcept { return {}; }
        Windows::UI::Xaml::Interop::TypeName Type() const noexcept { return {}; }

    protected:

        void load_markup(hstring const& uri)
        {
            Windows::UI::Xaml::Application::LoadComponent(*this, Uri(uri));
        }

        void property_changed(hstring const& name)
        {
            m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
        }

        template <typename T>
        auto bind_property(T const& value)
        {
            return make<property<T>>(value);
        }

    private:

        template <typename T>
        struct property : implements<property<T>, Windows::UI::Xaml::Data::ICustomProperty>
        {
            T const& m_value;
            property(T const& value) : m_value(value) {}

            Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const&) const
            {
                return box_value(m_value);
            }

            Windows::UI::Xaml::Interop::TypeName Type() const noexcept { return {}; }
            hstring Name() const noexcept { return {}; }
            void SetValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept {}
            Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept { return {}; }
            void SetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept {}
            bool CanWrite() const noexcept { return {}; }
            bool CanRead() const noexcept { return {}; }
        };

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };
}