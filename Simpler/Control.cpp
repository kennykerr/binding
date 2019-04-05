#include "pch.h"
#include "xaml.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;

struct MyControl : xaml_user_control<MyControl>
{
    hstring m_text{ L"0" };
    event_token m_loaded;

    MyControl()
    {
        DataContext(*this);

        m_loaded = Loaded([&](auto && ...)
            {
                Loaded(m_loaded);
                UpdateAsync();
            });

        InitializeComponent(L"ms-appx:///Control.xaml");
    }

    static hstring GetRuntimeClassName()
    {
        return L"Sample.MyControl";
    }

    static XamlTypeRegistry::MemberInformation GetMember(hstring const& name)
    {
        if (name == L"SampleText")
        {
            return {
                name,
                XamlTypeRegistry::GetType(L"String"),
                [](IInspectable instance) { return winrt::box_value(instance.as<MyControl>()->m_text); },
                nullptr };
        }

        return {};
    }

    fire_and_forget UpdateAsync()
    {
        using namespace std::literals;
        apartment_context context;
        int value{};

        while (true)
        {
            co_await 100ms;
            co_await context;

            m_text = std::to_wstring(++value);
            property_changed(L"SampleText");
        }
    }
};

static bool register_type{ XamlTypeRegistry::RegisterType<MyControl>() };
