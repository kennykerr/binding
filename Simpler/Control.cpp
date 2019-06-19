#include "pch.h"
#include "xaml.h"
#include "winrt/Windows.UI.Xaml.Hosting.h"

namespace winrt::impl
{
    // TODO: these bind_member<T> specializations should be generated by cppwinrt for all WinRT types

    template <> struct bind_member<Windows::Foundation::Uri>
    {
        static Windows::Foundation::IInspectable get(Windows::Foundation::Uri const& object, hstring const& name)
        {
            if (name.empty()) return make<bind_object<Windows::Foundation::Uri>>(object);
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
            WINRT_ASSERT(false);
        }
    };

    template <> struct bind_member<Windows::UI::Composition::SpriteVisual>
    {
        static Windows::Foundation::IInspectable get(Windows::UI::Composition::SpriteVisual const& object, hstring const& name)
        {
            if (name.empty()) return make<bind_object<Windows::UI::Composition::SpriteVisual>>(object);

            // TODO: because Offset is a struct, the bind_object must take a Setter and carry it along so that any 
            // call to "set" to update a field also calls the Setter to update the owning object.
            // Also maybe don't need to implement INotifyPropertyChanged on these?

            // TODO: just let bind_object take an optional delegate<void(T const)> !!!

            if (name == L"Offset") return make<bind_object<Windows::Foundation::Numerics::float3>>(object.Offset(), [object](auto&& value) { object.Offset(value); });

            if (name == L"Comment") return box_value(object.Comment());
            WINRT_ASSERT(false);
            return nullptr;
        }

        static void set(Windows::UI::Composition::SpriteVisual const& object, hstring const& name, Windows::Foundation::IInspectable const& value)
        {
            if (name == L"Comment") { object.Comment(unbox_value<hstring>(value)); return; };
            WINRT_ASSERT(false);
        }
    };

    //template <> struct bind_member<Windows::Foundation::Numerics::float3>
    //{
    //    static Windows::Foundation::IInspectable get(Windows::Foundation::Numerics::float3 const& object, hstring const& name)
    //    {
    //        // TODO: somehow overload box_value or add a bind_value helper that does make<bind_object... or box_value as necessary

    //        if (name.empty()) return make<bind_object<Windows::Foundation::Numerics::float3>>(object);
    //        if (name == L"x") return box_value(object.x);
    //        if (name == L"y") return box_value(object.y);
    //        if (name == L"z") return box_value(object.z);
    //        WINRT_ASSERT(false);
    //        return nullptr;
    //    }

    //    static void set(Windows::Foundation::Numerics::float3& object, hstring const& name, Windows::Foundation::IInspectable const& value)
    //    {
    //        if (name == L"x") { object.x = unbox_value<float>(value); return; };
    //        WINRT_ASSERT(false);
    //    }
    //};
}

namespace winrt
{
    xaml_binding bind(Windows::Foundation::Numerics::float3& object, hstring const& name)
    {
        if (name == L"x") return object.x;
        if (name == L"y") return object.y;
        if (name == L"z") return object.z;
        return {};
    }

    // TODO:
    // 1. get rid of bind_member. 
    // 2. Just use bind function.
    // 3. Update xaml_binding to take a pair of delegates and infer type?
    // 4. Use delegates rather than virtual accessor vtable.

    //xaml_binding bind(Windows::Foundation::Uri const& object, hstring const& name)
    //{
    //    if (name == L"Domain") return object.Domain();
    //    if (name == L"Port") return object.Port();
    //    return {};
    //}
}

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Composition;

struct StringToFloatConverter : registered_implements<StringToFloatConverter, Windows::UI::Xaml::Data::IValueConverter>
{
    static hstring type_name()
    {
        return L"Sample.StringToFloatConverter";
    }

    auto Convert(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, Windows::Foundation::IInspectable const&, param::hstring const&) const
    {
        return value;
    }

    auto ConvertBack(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&, Windows::Foundation::IInspectable const&, param::hstring const&) const
    {
        return box_value(std::stof(std::wstring(unbox_value<hstring>(value))));
    }
};

struct SampleControl : xaml_user_control<SampleControl>
{
    int m_counter{};
    hstring m_text;
    IObservableVector<int> m_list{ single_threaded_observable_vector<int>() };
    Uri m_uri{ L"http://kennykerr.ca/about" };
    SpriteVisual m_visual{ nullptr };

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

        if (name == L"Visual")
        {
            return m_visual;
        }

        return {};
    }

    SampleControl() : base_type(L"ms-appx:///Control.xaml")
    {
        auto compositor = ElementCompositionPreview::GetElementVisual(*this).Compositor();
        m_visual = compositor.CreateSpriteVisual();
        m_visual.Brush(compositor.CreateColorBrush({ 128, 255, 0, 0 }));
        m_visual.Size({ 100,100 });
        ElementCompositionPreview::SetElementChildVisual(*this, m_visual);
        m_visual.Comment(L"Default");

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
