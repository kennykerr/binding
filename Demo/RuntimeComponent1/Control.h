#pragma once
#include "Control.g.h"
#include "winrt/Windows.UI.Xaml.Controls.h"

namespace winrt::RuntimeComponent1::implementation
{
    struct Control : ControlT<Control>
    {
        Control() {
            Windows::UI::Xaml::Controls::TextBlock tb;
            tb.Text(L"C++/WinRT control");
            tb.FontSize(30.0f);
            Content(tb);
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        hstring Name();
    };
}
namespace winrt::RuntimeComponent1::factory_implementation
{
    struct Control : ControlT<Control, implementation::Control>
    {
    };
}
