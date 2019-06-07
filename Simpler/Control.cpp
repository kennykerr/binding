#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

namespace winrt
{
    xaml_member bind(Uri const& /*object*/, hstring const& /*name*/)
    {
        return {};
    }
}

struct Member
{
    xaml_member bind(hstring const& /*name*/)
    {
        return {};
    }
};

struct UriThing : xaml_type<UriThing, true, implements>
{
    static hstring GetRuntimeClassName() { return L"UriThing"; }

    xaml_member bind(hstring const& name)
    {
        if (name == L"Domain")
        {
            return m_domain;
        }

        return {};
    }

private:

    hstring m_domain{ L"Some string value" };
};

struct ViewModel : xaml_type<ViewModel, true, implements>
{
    static hstring GetRuntimeClassName() { return L"ViewModel"; }

    xaml_member bind(hstring const& name)
    {
        if (name == L"Uri")
        {
            return m_thing;
        }

        return {};
    }

private:

    UriThing m_thing;
};

struct SampleControl : xaml_user_control<SampleControl>
{
    int m_counter{};
    hstring m_text;
    IObservableVector<int> m_list{ observable_vector<int>() };
    Uri m_uri{ L"http://kennykerr.ca/about" };
    Member m_member;

    static hstring type_name()
    {
        return L"Sample.Control";
    }

    xaml_member bind(hstring const& name)
    {
        name;

        //if (name == L"Uri")
        //{
        //    return m_uri;
        //}

        //if (name == L"Counter")
        //{
        //    return m_counter;
        //}

        //if (name == L"Member")
        //{
        //    return m_member;
        //}

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
        DataContext(make<ViewModel>());
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
};
