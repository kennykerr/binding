#include "pch.h"
#include "xaml_binding.h"

using namespace winrt;
using namespace std::literals;

using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;

struct MainPage : xaml_binding<MainPage, PageT>
{
    MainPage()
    {
        load_markup(L"ms-appx:///MainPage.xaml");
    }
};

struct MyControl : xaml_binding<MyControl, UserControlT>
{
    static hstring GetRuntimeClassName()
    {
        return L"App.MyControl";
    }

    MyControl()
    {
        load_markup(L"ms-appx:///MyControl.xaml");
        UpdateAsync();
    }

    ICustomProperty bind(hstring const& name)
    {
        if (name == L"DisplayText")
        {
            return bind_property(m_text);
        }

        return nullptr;
    }

private:

    IAsyncAction UpdateAsync()
    {
        apartment_context context;
        int value{};

        while (true)
        {
            co_await 1s;
            co_await context;

            m_text = std::to_wstring(++value);
            property_changed(L"DisplayText");
        }
    }

    hstring m_text{ L"0" };

};

struct App : xaml_binding_app<App>
{
    App()
    {
        add_bindable_types<MyControl>();
    }

    void OnLaunched(LaunchActivatedEventArgs const&)
    {
        auto window = Window::Current();

        if (!window.Content())
        {
            window.Content(make<MainPage>());
        }

        window.Activate();
    }
};

int __stdcall wWinMain(void*, void*, wchar_t*, int)
{
    Application::Start([](auto &&) { make<App>(); });
}
