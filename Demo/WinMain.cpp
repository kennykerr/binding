#include "pch.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/RuntimeComponent1.h"
#include "xaml_host.h"

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

    Uri uri(L"http://aka.ms/cppwinrt");
    ::MessageBoxW(::GetDesktopWindow(), uri.AbsoluteUri().c_str(), L"C++/WinRT Application", MB_OK);

    XamlHost host;

    // 1. Simple demo of xaml host with C++/WinRT
    TextBlock tb;
    tb.Text(L"Hello TextBlock");
    tb.FontSize(30.0f);
    host.Content(tb);

    // 2. C++/WinRT local xaml control
    struct MyControl : UserControlT<MyControl>
    {
        MyControl()
        {
            TextBlock tb;
            tb.Text(L"Hello local control");
            tb.FontSize(30.0f);
            Content(tb);
        }
    };
    host.Content(make<MyControl>());

    // 3. Runtime component UWP control, via Reg-Free WinRT and VCRT forwarders
    RuntimeComponent1::BlankUserControl c;
    host.Content(c);
    
    host.Run();
}
