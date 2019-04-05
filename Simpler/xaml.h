// This header would become "winrt/xaml.h"

#pragma once

#include "XamlTypeRegistry.h"

namespace winrt
{
    template <typename D, typename... I>
    struct xaml_app :
        Windows::UI::Xaml::ApplicationT<D, Windows::UI::Xaml::Markup::IXamlMetadataProvider, I...>
    {
        Windows::UI::Xaml::Markup::IXamlType GetXamlType(Windows::UI::Xaml::Interop::TypeName const& type) const
        {
            return GetXamlType(type.Name);
        }

        Windows::UI::Xaml::Markup::IXamlType GetXamlType(hstring const& fullName) const
        {
            return XamlTypeRegistry::GetType(fullName);
        }

        com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions() const
        {
            return {};
        }
    };

    template <typename D, template <typename...> typename B, typename... I>
    struct xaml_bindable_type : B<D, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
    {
        // INotifyPropertyChanged
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler) { return m_changed.add(handler); }
        void PropertyChanged(winrt::event_token token) { m_changed.remove(token); }

    protected:

        void InitializeComponent(hstring const& uri) { Windows::UI::Xaml::Application::LoadComponent(*this, Windows::Foundation::Uri(uri)); }
        void property_changed(hstring const& name) { m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name)); }

    private:

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };

    template <typename D>
    using xaml_page = xaml_bindable_type<D, Windows::UI::Xaml::Controls::PageT>;

    template <typename D>
    using xaml_user_control = xaml_bindable_type<D, Windows::UI::Xaml::Controls::UserControlT>;
}
