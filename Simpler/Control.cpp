#include "pch.h"
#include "xaml.h"

namespace winrt::impl
{
    template <> struct binding<Windows::Foundation::Uri>
    {
        static Windows::Foundation::IInspectable get(Windows::Foundation::Uri const& object, hstring const& name)
        {
            if (name == L"Domain") return box_value(object.Domain());
            if (name == L"Port") return box_value(object.Port());
            WINRT_ASSERT(false);
            return nullptr;
        }

        static void set(Windows::Foundation::Uri const& object, hstring const& name, Windows::Foundation::IInspectable const& value)
        {
            object;
            name;
            value;

            // TODO: only add if (...) for properties that are writable

            // You seem to have an invalid binding expression.
            WINRT_ASSERT(false);
        }
    };
}

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;


template <typename T>
struct uri_domain : implements<uri_domain<T>, Windows::UI::Xaml::Data::ICustomProperty>
{
    T m_object;
    hstring m_name;

    uri_domain(T const& object, hstring const& name) :
        m_object(object),
        m_name(name)
    {
    }

    Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const&) const
    {
        return winrt::impl::binding<T>::get(m_object, m_name);
    }
    void SetValue(Windows::Foundation::IInspectable const&, [[maybe_unused]] Windows::Foundation::IInspectable const& value) const noexcept
    {
        winrt::impl::binding<T>::set(m_object, m_name, value);
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
    Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
    {

        return nullptr;
    }
    void SetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept
    {
    }
};

template <typename T>
struct uri_provider : implements<uri_provider<T>, Windows::UI::Xaml::Data::ICustomPropertyProvider, Windows::UI::Xaml::Data::INotifyPropertyChanged>
{
    T m_object;

    event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        handler;
        return {};
    }

    void PropertyChanged(winrt::event_token )
    {
        
    }

    uri_provider(T const& object) :
        m_object(object)
    {
    }

    Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
    {
        name;
        return make<uri_domain<T>>(m_object, name);
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
        return make<uri_provider<Uri>>(object);
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

        if (name == L"Uri")
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
