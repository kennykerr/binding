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

template <typename T, auto Getter, auto Setter = nullptr>
struct uri_domain : implements<uri_domain<T, Getter, Setter>, Windows::UI::Xaml::Data::ICustomProperty>
{
    T m_object;

    uri_domain(T const& object) :
        m_object(object)
    {
    }

    Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const&) const
    {
        //return box_value(m_object.Domain());

        // TODO: maybe use is_member_object_pointer to decide whether Getter is member variable rather than member function
        // for structs
        static_assert(std::is_member_function_pointer_v<decltype(Getter)>);

        // Then create constexpr tables of bind info for each type and avoid having actual bind function overloads everywhere.

        return box_value(((&m_object)->*(Getter))());
    }
    void SetValue(Windows::Foundation::IInspectable const&, [[maybe_unused]] Windows::Foundation::IInspectable const& value) const noexcept
    {
        if constexpr (Setter != nullptr)
        {
            ((&m_object)->*(Setter))(unbox_value<hstring>(value));
        }
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
struct uri_provider : implements<uri_provider<T>, Windows::UI::Xaml::Data::ICustomPropertyProvider>
{
    T m_object;

    uri_provider(T const& object) :
        m_object(object)
    {
    }

    Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name) const
    {
        name;
        return make<uri_domain<T, &Uri::Domain>>(m_object);
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
