#pragma once

#include "xaml_bindable_type.h"
#include "XamlTypeRegistry.h"

namespace Sample3::DependencyProperty
{

    struct ViewModel : winrt::xaml_bindable_type<ViewModel, winrt::implements>
    {
        static winrt::hstring GetRuntimeClassName();
        static winrt::Windows::UI::Xaml::Markup::IXamlType GetBaseType();
        static winrt::XamlTypeRegistry::MemberInformation GetMember(winrt::hstring const& name);

        ViewModel();

        winrt::hstring get_DisplayText();
        void put_DisplayText(winrt::hstring const& value);

    private:

        winrt::hstring m_text{ L"0" };
        winrt::Windows::Foundation::IAsyncAction UpdateAsync();
    };
}