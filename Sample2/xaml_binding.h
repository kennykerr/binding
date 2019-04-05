#pragma once

namespace winrt
{
    template <typename D, typename... I>
    struct xaml_binding_app : Windows::UI::Xaml::ApplicationT<D, Windows::UI::Xaml::Markup::IXamlMetadataProvider, I...>
    {

        Windows::UI::Xaml::Markup::IXamlType GetXamlType(hstring const& fullName) const
        {
            auto make_type = m_bindable_types.find(fullName);

            if (make_type == m_bindable_types.end())
            {
                return nullptr;
            }

            return make_type->second();
        }

        template <typename T> // TODO: make variadic
        void add_bindable_types()
        {
            m_bindable_types[T::GetRuntimeClassName()] = []
            {
                return make<xaml_binding_type<T>>();
            };
        }

        Windows::UI::Xaml::Markup::IXamlType GetXamlType(Windows::UI::Xaml::Interop::TypeName const& /*type*/) const noexcept { return {}; }
        com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions() const noexcept { return {}; }

    private:

        template <typename T>
        struct xaml_binding_type : implements<xaml_binding_type<T>, Windows::UI::Xaml::Markup::IXamlType>
        {
            Windows::Foundation::IInspectable ActivateInstance() const
            {
                return make<T>();
            }

            Windows::UI::Xaml::Markup::IXamlType BaseType() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlMember ContentProperty() const noexcept { return {}; }
            hstring FullName() const noexcept { return {}; }
            bool IsArray() const noexcept { return {}; }
            bool IsCollection() const noexcept { return {}; }
            bool IsConstructible() const noexcept { return {}; }
            bool IsDictionary() const noexcept { return {}; }
            bool IsMarkupExtension() const noexcept { return {}; }
            bool IsBindable() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType ItemType() const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlType KeyType() const noexcept { return {}; }
            Windows::UI::Xaml::Interop::TypeName UnderlyingType() const noexcept { return {}; }
            Windows::Foundation::IInspectable CreateFromString(hstring const& /*value*/) const noexcept { return {}; }
            Windows::UI::Xaml::Markup::IXamlMember GetMember(hstring const& /*name*/) const noexcept { return {}; }
            void AddToVector(Windows::Foundation::IInspectable const& /*instance*/, Windows::Foundation::IInspectable const& /*value*/) const noexcept {}
            void AddToMap(Windows::Foundation::IInspectable const& /*instance*/, Windows::Foundation::IInspectable const& /*key*/, Windows::Foundation::IInspectable const& /*value*/) const noexcept {}
            void RunInitializer() const noexcept {}
        };

        using make_bindable_type = Windows::UI::Xaml::Markup::IXamlType(*)();
        std::map<winrt::hstring, make_bindable_type> m_bindable_types;
    };

    template <typename D, template <typename...> typename B, typename... I>
    struct xaml_binding : B<D, Windows::UI::Xaml::Data::ICustomPropertyProvider, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
    {
        xaml_binding()
        {
            this->DataContext(*this); // TODO: circular reference?
        }

        Windows::UI::Xaml::Data::ICustomProperty bind(hstring const&) const
        {
            return nullptr;
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
