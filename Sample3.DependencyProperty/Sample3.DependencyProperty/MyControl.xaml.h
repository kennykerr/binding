#pragma once

#include "xaml_bindable_type.h"
#include "XamlTypeRegistry.h"

namespace Sample3::DependencyProperty
{
    struct MyControl :
        winrt::xaml_bindable_type<MyControl, winrt::Windows::UI::Xaml::Controls::ControlT>
    {
        static winrt::hstring GetRuntimeClassName();
        static winrt::Windows::UI::Xaml::Markup::IXamlType GetBaseType();
        static winrt::XamlTypeRegistry::MemberInformation GetMember(winrt::hstring const& name);

        MyControl();

        winrt::hstring get_Message();
        void put_Message(winrt::hstring const& value);

    private:

        static const winrt::Windows::UI::Xaml::DependencyProperty s_MessageProperty;
    };
}