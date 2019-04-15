#include "pch.h"
#include "Control.h"
#include "Control.g.cpp"

namespace winrt::RuntimeComponent1::implementation
{
    int32_t Control::MyProperty()
    {
        return 42;
    }
    void Control::MyProperty(int32_t value)
    {
        value;
    }
    hstring Control::Name()
    {
        return L"Name";
    }
}
