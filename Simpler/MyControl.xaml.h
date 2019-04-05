#pragma once

#include "xaml_bindable_type.h"
#include "XamlTypeRegistry.h"

namespace Sample2
{
    struct MyControl :
        winrt::xaml_bindable_type<MyControl, winrt::Windows::UI::Xaml::Controls::UserControlT>
    {
        static winrt::hstring GetRuntimeClassName();
        static winrt::XamlTypeRegistry::MemberInformation GetMember(winrt::hstring const& name);

        MyControl();

        winrt::hstring get_DisplayText();
        void put_DisplayText(winrt::hstring const& value);

    private:

        winrt::event_token m_loadedEventToken;
        void OnLoaded(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& args);

        winrt::hstring m_text{ L"0" };
        winrt::Windows::Foundation::IAsyncAction UpdateAsync();
    };
}