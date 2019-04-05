#include "pch.h"
#include "xaml.h"

using namespace winrt;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml;

struct MyPage : xaml_page<MyPage>
{
    MyPage()
    {
        InitializeComponent(L"ms-appx:///MainPage.xaml");
    }
};

struct App : xaml_app<App>
{
    void OnLaunched(LaunchActivatedEventArgs const&)
    {
        auto window = Window::Current();
        window.Content(make<MyPage>());
        window.Activate();
    }
};

int __stdcall wWinMain(void*, void*, wchar_t*, int)
{
    Application::Start([](auto&&) { make<App>(); });
}
