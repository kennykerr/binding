#include "pch.h"
#include "xaml.h"

namespace winrt
{
    binding bind(Windows::Foundation::Uri const& object, hstring const& name)
    {
        if (name == L"Domain") return
        {
            [object]
            {
                return object.Domain();
            }
        };

        return {};
    }

    binding bind(Windows::Foundation::Numerics::float3& object, hstring const& name)
    {
        if (name == L"x") return object.x;
        if (name == L"y") return object.y;
        if (name == L"z") return object.z;
        return {};
    }

    binding bind(Windows::UI::Composition::SpriteVisual const& object, hstring const& name)
    {
        if (name == L"Comment") return
        {
            [object]
            {
                return object.Comment();
            },
            [object](auto&& value)
            {
                object.Comment(value);
            }
        };

        if (name == L"Offset") return
        {
            [object]
            {
                return object.Offset();
            },
            [object](auto&& value)
            {
                object.Offset(value);
            }
        };

        return {};
    }
}

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Composition;

struct SampleControl : xaml_user_control<SampleControl>
{
    int m_counter{};
    hstring m_text{ L"Text" };
    IObservableVector<int> m_list{ single_threaded_observable_vector<int>() };
    Uri m_uri{ L"http://kennykerr.ca/about" };
    SpriteVisual m_visual{ nullptr };

    static hstring type_name()
    {
        return L"Sample.Control";
    }

    binding bind(hstring const& name)
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
