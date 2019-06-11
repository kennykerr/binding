#include "pch.h"
#include "xaml.h"

namespace winrt::impl
{
    // TODO: this should be generated by cppwinrt.exe

    template <> struct bind_member<Windows::Foundation::Uri>
    {
        hstring name;

        Windows::Foundation::IInspectable get(Windows::Foundation::Uri const& object) const
        {
            if (name.empty()) return make<bind_object<Windows::Foundation::Uri>>(object);
            if (name == L"Domain") return box_value(object.Domain());
            if (name == L"Port") return box_value(object.Port());
            WINRT_ASSERT(false);
            return nullptr;
        }

        void set(Windows::Foundation::Uri const& object, Windows::Foundation::IInspectable const& value) const
        {
            object;
            value;
            WINRT_ASSERT(false);
        }
    };
}


using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;



//namespace winrt
//{
//    xaml_binding bind(Uri const& object, hstring const& name)
//    {
//        object;
//        name;
//        return {};
//    }
//
//    Windows::Foundation::IInspectable box_value(Uri const& object)
//    {
//        return make<impl::bind_object<Uri>>(object);
//    }
//}
//
//struct MemberSample
//{
//    xaml_binding bind(hstring const& name)
//    {
//        name;
//        return {};
//    }
//};

struct SampleControl : xaml_user_control<SampleControl>
{
    int m_counter{};
    hstring m_text;
    IObservableVector<int> m_list{ single_threaded_observable_vector<int>() };
    Uri m_uri{ L"http://kennykerr.ca/about" };
    //MemberSample m_member;

    static hstring type_name()
    {
        return L"Sample.Control";
    }

    xaml_binding bind(hstring const& name)
    {
        if (name == L"Counter")
        {
            return m_counter;
        }

        if (name == L"Uri")
        {
            return m_uri;
        }

        if (name == L"Text")
        {
            return m_text;
        }

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
