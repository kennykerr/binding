﻿#include "pch.h"
#include "BlankUserControl.h"
#if __has_include("BlankUserControl.g.cpp")
#include "BlankUserControl.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::RuntimeComponent1::implementation
{
    BlankUserControl::BlankUserControl()
    {
        InitializeComponent();
    }

    int32_t BlankUserControl::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void BlankUserControl::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
