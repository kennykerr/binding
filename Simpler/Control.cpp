#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

struct c_provider : implements<c_provider, Windows::UI::Xaml::Data::ICustomPropertyProvider>
{
    Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
    {
        name;
        return nullptr;
    }
    Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const&, Windows::UI::Xaml::Interop::TypeName const&) const noexcept
    {
        return nullptr;
    }
    hstring GetStringRepresentation() const noexcept
    {
        return {};
    }
    Windows::UI::Xaml::Interop::TypeName Type() const noexcept
    {
        return {};
    }
};

struct uri_domain : implements<uri_domain, Windows::UI::Xaml::Data::ICustomProperty>
{
    Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const& target) const
    {
        auto s = get_class_name(target);

        target;
        return box_value(L"Domain!");

        //return make<c_provider>();
    }
    bool CanWrite() const noexcept
    {
        return false;
    }
    bool CanRead() const noexcept
    {
        return true;
    }

    Windows::UI::Xaml::Interop::TypeName Type() const noexcept
    {
        return {};
    }
    hstring Name() const noexcept
    {
        return {};
    }
    void SetValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
    {
    }
    Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
    {

        return nullptr;
    }
    void SetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
    {
    }
};

struct uri_provider : implements<uri_provider, Windows::UI::Xaml::Data::ICustomPropertyProvider>
{
    Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
    {
        name;
        return make<uri_domain>();
    }
    Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const&, Windows::UI::Xaml::Interop::TypeName const&) const noexcept
    {
        return nullptr;
    }
    hstring GetStringRepresentation() const noexcept
    {
        return {};
    }
    Windows::UI::Xaml::Interop::TypeName Type() const noexcept
    {
        return {};
    }
};

namespace winrt
{
    xaml_member bind(Uri const& object, hstring const& name)
    {
        object;
        name;
        return {};
    }

    Windows::Foundation::IInspectable box_value(Uri const& object)
    {
        return make<uri_provider>();
    }
}

struct MemberSample
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
    MemberSample m_member;

    static hstring type_name()
    {
        return L"Sample.Control";
    }

    xaml_member bind(hstring const& name)
    {
        if (name == L"Counter")
        {
            return m_counter;
        }

        if (name == L"A")
        {
            return m_uri;
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
