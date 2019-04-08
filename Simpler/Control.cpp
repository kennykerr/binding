#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;

struct MyControl : xaml_user_control<MyControl>
{
    hstring m_text{ L"0" };

    MyControl() : base_type(L"ms-appx:///Control.xaml")
    {
        Loaded([&](auto&&...) { UpdateAsync(); });
    }

    static hstring GetRuntimeClassName()
    {
        return L"Sample.MyControl";
    }

    static xaml_member_info get_member(hstring const& name)
    {
        if (name == L"SampleText")
        {
            return
            {
                xaml_registry::get(L"String"),
                [](auto&& instance) { return box_value(instance.as<MyControl>()->m_text); }
            };
        }

        return {};
    }

    fire_and_forget UpdateAsync()
    {
        int value{};

        while (true)
        {
            co_await 100ms;
            co_await resume_foreground(Dispatcher());

            m_text = std::to_wstring(++value);
            property_changed(L"SampleText");
        }
    }
};

static bool registration{ xaml_registry::add<MyControl>() };
