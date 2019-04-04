#include "pch.h"

#include "xaml_metadataprovider_app.h"

#include "MainPage.xaml.h"

using namespace winrt;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml;

namespace Sample2
{
    struct App : xaml_metadataprovider_app<App>
    {
        void OnLaunched(LaunchActivatedEventArgs const&)
        {
            auto window = Window::Current();
            window.Content(make<MainPage>());
            window.Activate();
        }
    };
}

int __stdcall wWinMain(void*, void*, wchar_t*, int)
{
    Application::Start([](auto &&) { make<Sample2::App>(); });
}