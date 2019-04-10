#include "pch.h"

#include "MyControl.xaml.h"

#include "XamlTypeRegistry.h"

#include <winrt\Windows.UI.Xaml.Interop.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Markup;

using namespace Sample3::DependencyProperty;

static bool s_bMyControlRegistered = XamlTypeRegistry::RegisterType<MyControl>();

const DependencyProperty MyControl::s_MessageProperty = DependencyProperty::Register(
    L"Message",
    { L"String", TypeKind::Primitive },
    { L"Sample3.DependencyProperty.MyControl", TypeKind::Custom },
    nullptr);

hstring MyControl::GetRuntimeClassName() { return L"Sample3.DependencyProperty.MyControl"; }

IXamlType MyControl::GetBaseType() { return XamlTypeRegistry::GetType(L"Windows.UI.Xaml.Controls.Control"); }

XamlTypeRegistry::MemberInformation MyControl::GetMember(hstring const& name)
{
    if (name == L"Message")
    {
        return {
            name,
            XamlTypeRegistry::GetType(L"String"),
            [](IInspectable instance) { return winrt::box_value(instance.as<MyControl>()->get_Message()); },
            [](IInspectable instance, IInspectable value) { instance.as<MyControl>()->put_Message(winrt::unbox_value<hstring>(value)); }
            };
    }

    return {};
}

MyControl::MyControl() { InitializeComponent(L"ms-appx:///MyControl.xaml"); }

hstring MyControl::get_Message()
{
    return winrt::unbox_value<hstring>(GetValue(MyControl::s_MessageProperty));
}

void MyControl::put_Message(hstring const& value)
{
    SetValue(MyControl::s_MessageProperty, winrt::box_value(value));
}