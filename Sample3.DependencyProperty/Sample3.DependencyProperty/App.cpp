#include "pch.h"

#include "xaml_metadataprovider_app.h"

#include "MainPage.xaml.h"

using namespace winrt;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace Sample3::DependencyProperty
{
    struct App : xaml_metadataprovider_app<App>
    {
        void OnLaunched(LaunchActivatedEventArgs const&)
        {
            UnhandledException(
                [](IInspectable const&, UnhandledExceptionEventArgs const& e)
                {
                    auto errorMessage = e.Message();
                });
            this->DebugSettings().BindingFailed(
                [](IInspectable const&, BindingFailedEventArgs const& e)
                {
                    auto message = e.Message();
                });

            auto window = Window::Current();
            window.Content(make<MainPage>());
            window.Activate();
        }
    };
}

int __stdcall wWinMain(void*, void*, wchar_t*, int)
{
    Application::Start([](auto &&) { make<Sample3::DependencyProperty::App>(); });
}