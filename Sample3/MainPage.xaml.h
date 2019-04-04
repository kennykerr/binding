#pragma once

#include "xaml_bindable_type.h"

namespace Sample2
{
    struct MainPage :
        winrt::xaml_bindable_type<MainPage, winrt::Windows::UI::Xaml::Controls::PageT>
    {
        MainPage();
    };
}