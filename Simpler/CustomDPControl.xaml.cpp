#include "pch.h"
#include "winrt/xaml.h"

using namespace winrt;

using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;

namespace Simple
{
    struct CustomDPControl : xaml_control<CustomDPControl>
    {
        static hstring type_name() { return L"Sample.CustomDPControl"; }

        SolidColorBrush GetSelectedBrush()
        {
            return GetValue(CustomDPControl::SelectedBrushProperty).as<SolidColorBrush>();
        }
        void SetSelectedBrush(SolidColorBrush const& value)
        {
            SetValue(CustomDPControl::SelectedBrushProperty, value);
        }

        CustomDPControl() : base_type(L"ms-appx:///CustomDPControl.xaml")
        {
            SetSelectedBrush(SolidColorBrush(Colors::Green()));
        }

        xaml_binding bind(hstring const& name)
        {
            if (name == L"SelectedBrush")
            {
                // does this capture strong refs?
                return xaml_binding(
                    [this] {
                        return GetSelectedBrush();
                    },
                    [this] (SolidColorBrush const& value) {
                        SetSelectedBrush(value);
                    });
            }

            return {};
        }

    private:

        static DependencyProperty SelectedBrushProperty;
    };

    DependencyProperty CustomDPControl::SelectedBrushProperty = DependencyProperty::Register(
        L"SelectedBrush", // name
        xaml_typename<SolidColorBrush>(), // propertyType
        { CustomDPControl::type_name(), TypeKind::Metadata }, // ownerType
        nullptr // typeMetadata
        //PropertyMetadata(SolidColorBrush(Colors::Green())) // typeMetadata
        );
}