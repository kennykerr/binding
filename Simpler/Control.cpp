#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

namespace winrt
{
    xaml_member bind(Uri const& object, hstring const& name)
    {
        object;
        name;
        return {};
    }
}

struct Member
{
    xaml_member bind(hstring const& name)
    {
        name;
        return {};
    }
};

struct SampleControl : xaml_user_control<SampleControl>
{
    int m_counter{};
    hstring m_text;
    IObservableVector<int> m_list{ single_threaded_observable_vector<int>() };
    Uri m_uri{ L"http://kennykerr.ca/about" };
    Member m_member;

    static hstring type_name()
    {
        return L"Sample.Control";
    }

    xaml_member bind(hstring const& name)
    {
        if (name == L"Uri")
        {
            return m_uri;
        }

        if (name == L"Counter")
        {
            return m_counter;
        }

        if (name == L"Member")
        {
            return m_member;
        }

        //if (name == L"Text")
        //{
        //    return m_text;
        //}

        //if (name == L"List")
        //{
        //    return m_list;
        //}

        return {};
    }

    SampleControl() : base_type(L"ms-appx:///Control.xaml")
    {
        DataContext(*this);
        Loaded([&](auto && ...)
            {
                UpdateAsync();
            });
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
};
