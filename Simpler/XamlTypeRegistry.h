#pragma once

namespace winrt
{
    struct XamlTypeRegistry
    {
        using CreateInstance = std::function<Windows::Foundation::IInspectable()>;

        struct MemberInformation
        {
            using GetValue = std::function<Windows::Foundation::IInspectable(Windows::Foundation::IInspectable)>;
            using SetValue = std::function<void(Windows::Foundation::IInspectable, Windows::Foundation::IInspectable)>;

            hstring MemberName;
            Windows::UI::Xaml::Markup::IXamlType MemberType;
            GetValue Getter;
            SetValue Setter;
        };
        using GetMemberInformation = std::function<MemberInformation(hstring const&)>;

        struct TypeInformation
        {
            hstring TypeName;
            CreateInstance CreateInstance;
            Windows::UI::Xaml::Markup::IXamlType BaseType;
            GetMemberInformation GetMember;
        };

        template <typename T>
        static bool RegisterType()
        {
            return RegisterType({
                T::GetRuntimeClassName(),
                [] { return make<T>().as<Windows::Foundation::IInspectable>(); },
                GetType(L"Object"), // TODO : Page, UserControl, etc.
                [](hstring const& memberName) { return T::GetMember(memberName); }
                });
        }

        static bool RegisterType(TypeInformation const& typeInfo);

        static Windows::UI::Xaml::Markup::IXamlType GetType(hstring const& typeName);
    };
}