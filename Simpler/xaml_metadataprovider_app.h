#pragma once

#include "XamlTypeRegistry.h"

namespace winrt
{
    template <typename D, typename... I>
    struct xaml_metadataprovider_app :
        Windows::UI::Xaml::ApplicationT<D, Windows::UI::Xaml::Markup::IXamlMetadataProvider, I...>
    {
        Windows::UI::Xaml::Markup::IXamlType GetXamlType(
            Windows::UI::Xaml::Interop::TypeName const& type) const
        {
            return GetXamlType(type.Name);
        }

        Windows::UI::Xaml::Markup::IXamlType GetXamlType(
            hstring const& fullName) const
        {
            return XamlTypeRegistry::GetType(fullName);
        }

        com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions() const
        {
            return {};
        }
    };
}