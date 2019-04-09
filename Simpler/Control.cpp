#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

struct MyControl : xaml_user_control<MyControl>
{
    int m_counter{};
    hstring m_text;
    IObservableVector<inspectable> m_list{ observable_vector<inspectable>() };

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

        if (name == L"List")
        {
            return m_list;
        }

        return {};
    }

    fire_and_forget UpdateAsync()
    {
        while (true)
        {
            co_await 1s;
            co_await resume_foreground(Dispatcher());

            ++m_counter;
            property_changed(L"Counter");

            m_list.Append(box_value(m_counter));
        }
    }
};

static bool registration{ xaml_registry::add<MyControl>() };
