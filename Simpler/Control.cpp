#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

struct SampleControl : xaml_user_control<SampleControl>
{
    int m_counter{};
    hstring m_text;
    IObservableVector<boxed_value> m_list{ observable_vector<boxed_value>() };

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

    SampleControl() : base_type(L"ms-appx:///Control.xaml")
    {
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

            m_list.Append(m_counter);
        }
    }

    static hstring type_name()
    {
        return L"Sample.Control";
    }
};
