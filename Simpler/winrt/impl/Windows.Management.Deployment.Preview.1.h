// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.3.4.5

#ifndef WINRT_Windows_Management_Deployment_Preview_1_H
#define WINRT_Windows_Management_Deployment_Preview_1_H
#include "winrt/impl/Windows.Management.Deployment.Preview.0.h"
namespace winrt::Windows::Management::Deployment::Preview
{
    struct __declspec(empty_bases) IClassicAppManagerStatics :
        Windows::Foundation::IInspectable,
        impl::consume_t<IClassicAppManagerStatics>
    {
        IClassicAppManagerStatics(std::nullptr_t = nullptr) noexcept {}
        IClassicAppManagerStatics(void* ptr, take_ownership_from_abi_t) noexcept : Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) IInstalledClassicAppInfo :
        Windows::Foundation::IInspectable,
        impl::consume_t<IInstalledClassicAppInfo>
    {
        IInstalledClassicAppInfo(std::nullptr_t = nullptr) noexcept {}
        IInstalledClassicAppInfo(void* ptr, take_ownership_from_abi_t) noexcept : Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif