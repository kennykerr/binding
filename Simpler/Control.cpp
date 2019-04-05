#include "pch.h"
#include "xaml.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;

struct MyControl : xaml_user_control<MyControl>
{
    hstring m_text{ L"0" };

    MyControl()
    {
        DataContext(*this);
        InitializeComponent(L"ms-appx:///Control.xaml");
        Loaded([&](auto && ...) { UpdateAsync(); });
    }

    static hstring GetRuntimeClassName()
    {
        return L"Sample.MyControl";
    }

    fire_and_forget UpdateAsync()
    {
        apartment_context context;
        int value{};

        while (true)
        {
            using namespace std::literals;
            co_await 100ms;
            co_await context;

            m_text = std::to_wstring(++value);
            property_changed(L"SampleText");
        }
    }

    static xaml_registry::member_info GetMember(hstring const& name)
    {
        if (name == L"SampleText")
        {
            return {
                name,
                xaml_registry::get(L"String"),
                [](IInspectable instance) { return winrt::box_value(instance.as<MyControl>()->m_text); },
                nullptr };
        }

        return {};
    }
};

static bool register_type{ xaml_registry::add<MyControl>() };
