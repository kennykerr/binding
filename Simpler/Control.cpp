#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;

struct MyControl : xaml_user_control<MyControl>
{
    int m_counter{};
    hstring m_text;

    MyControl() : base_type(L"ms-appx:///Control.xaml")
    {
        Loaded([&](auto&&...) { UpdateAsync(); });
    }

    static hstring GetRuntimeClassName()
    {
        return L"Sample.MyControl";
    }

    xaml_member bind(hstring const& name)
    {
        if (name == L"Counter")
        {
            return m_counter;
        }

        if (name == L"Text")
        {
            return m_text;
        }

        return {};
    }

    fire_and_forget UpdateAsync()
    {
        while (true)
        {
            co_await 100ms;
            co_await resume_foreground(Dispatcher());

            ++m_counter;
            property_changed(L"Counter");
        }
    }
};

static bool registration{ xaml_registry::add<MyControl>() };
