#include "pch.h"
#include "winrt/Windows.UI.Core.h"

using namespace winrt;
using namespace std::literals;

struct DemoControl : xaml_user_control<DemoControl>
{
	DemoControl() : base_type(L"ms-appx:///Control.xaml")
	{
		DataContext(*this);
		Loaded([&](auto && ...) { UpdateAsync(); });
	}

	fire_and_forget UpdateAsync()
	{
		while (true)
		{
			co_await 500ms;
			co_await resume_foreground(Dispatcher());

			++m_counter;
			property_changed(L"Counter");
		}
	}

    static hstring type_name()
    {
        return L"Demo.Control";
    }

    int m_counter{};
    hstring m_text;

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
};
