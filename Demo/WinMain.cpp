#include "pch.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/RuntimeComponent1.h"
#include "XamlIsland.h"

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    init_apartment(apartment_type::single_threaded);

    XamlIsland island;

    // 1. Simple demo of xaml island with C++/WinRT
    TextBlock tb;
    tb.Text(L"Hello Desktop XAML, from TextBlock");
    tb.FontSize(30.0f);
    island.Content(tb);

    // 2. C++/WinRT local xaml control
    struct MyControl : UserControlT<MyControl, IInspectable>
    {
        MyControl()
        {
            TextBlock tb;
            tb.Text(L"Hello Desktop XAML, from local control");
            tb.FontSize(30.0f);
            Content(tb);
        }
    };
    island.Content(make<MyControl>());


    // 3. Runtime component UWP control, via Reg-Free WinRT and VCRT forwarders
    RuntimeComponent1::BlankUserControl c;
    island.Content(c);
    
    island.Run();
}
