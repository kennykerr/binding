#include "pch.h"

#include "ViewModel.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Markup;

using namespace Sample3::DependencyProperty;

static bool s_bMyControlRegistered = XamlTypeRegistry::RegisterType<ViewModel>();

hstring ViewModel::GetRuntimeClassName() { return L"Sample3.DependencyProperty.ViewModel"; }

IXamlType ViewModel::GetBaseType() { return XamlTypeRegistry::GetType(L"Object"); }

XamlTypeRegistry::MemberInformation ViewModel::GetMember(hstring const& name)
{
    if (name == L"DisplayText")
    {
        return {
            name,
            XamlTypeRegistry::GetType(L"String"),
            [](IInspectable instance) { return winrt::box_value(instance.as<ViewModel>()->get_DisplayText()); },
            nullptr
            };
    }

    return {};
}

ViewModel::ViewModel() { UpdateAsync(); }

hstring ViewModel::get_DisplayText() { return m_text; }

void ViewModel::put_DisplayText(hstring const& value)
{
    if (m_text != value)
    {
        m_text = value;
        property_changed(L"DisplayText");
    }
}

IAsyncAction ViewModel::UpdateAsync()
{
    apartment_context context;
    int value{};

    while (true)
    {
        co_await 1s;
        co_await context;

        put_DisplayText(hstring(std::to_wstring(++value)));
    }
}