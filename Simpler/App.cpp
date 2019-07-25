#include "pch.h"
#include "winrt/xaml.h"

using namespace winrt;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml;

struct MainPage : xaml_page<MainPage>
{
    MainPage() : base_type(L"ms-appx:///MainPage.xaml")
    {
    }
};

struct App : xaml_app<App>
{
    App()
    {
        UnhandledException(
            [](Windows::Foundation::IInspectable const&, Windows::UI::Xaml::UnhandledExceptionEventArgs const& e)
            {
                hstring message{ e.Message() };
                (void)message;
            });
    }

    void OnLaunched(LaunchActivatedEventArgs const&)
    {
        auto window = Window::Current();
        window.Content(make<MainPage>());
        window.Activate();
    }
};

int __stdcall wWinMain(void*, void*, wchar_t*, int)
{
    Application::Start([](auto&&) { make<App>(); });
}
