#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

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
