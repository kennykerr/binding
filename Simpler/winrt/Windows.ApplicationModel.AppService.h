// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.3.4.5

#ifndef WINRT_Windows_ApplicationModel_AppService_H
#define WINRT_Windows_ApplicationModel_AppService_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.3.4.5"), "Mismatched C++/WinRT headers.");
#include "winrt/Windows.ApplicationModel.h"
#include "winrt/impl/Windows.ApplicationModel.2.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Foundation.Collections.2.h"
#include "winrt/impl/Windows.System.2.h"
#include "winrt/impl/Windows.System.RemoteSystems.2.h"
#include "winrt/impl/Windows.ApplicationModel.AppService.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceCatalogStatics<D>::FindAppServiceProvidersAsync(param::hstring const& appServiceName) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceCatalogStatics)->FindAppServiceProvidersAsync(*(void**)(&appServiceName), &operation));
        return Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Windows::ApplicationModel::AppInfo>>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceClosedEventArgs<D>::Status() const
    {
        Windows::ApplicationModel::AppService::AppServiceClosedStatus value;
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceClosedEventArgs)->get_Status(put_abi(value)));
        return value;
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::AppServiceName() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->get_AppServiceName(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::AppServiceName(param::hstring const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->put_AppServiceName(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::PackageFamilyName() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->get_PackageFamilyName(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::PackageFamilyName(param::hstring const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->put_PackageFamilyName(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::OpenAsync() const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->OpenAsync(&operation));
        return Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceConnectionStatus>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::SendMessageAsync(Windows::Foundation::Collections::ValueSet const& message) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->SendMessageAsync(*(void**)(&message), &operation));
        return Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceResponse>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::RequestReceived(Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::AppService::AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs> const& handler) const
    {
        winrt::event_token token;
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->add_RequestReceived(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::RequestReceived_revoker consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::RequestReceived(auto_revoke_t, Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::AppService::AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, RequestReceived_revoker>(this, RequestReceived(handler));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::RequestReceived(winrt::event_token const& token) const noexcept
    {
        WINRT_VERIFY_(0, WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->remove_RequestReceived(impl::bind_in(token)));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::ServiceClosed(Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::AppService::AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceClosedEventArgs> const& handler) const
    {
        winrt::event_token token;
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->add_ServiceClosed(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::ServiceClosed_revoker consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::ServiceClosed(auto_revoke_t, Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::AppService::AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceClosedEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, ServiceClosed_revoker>(this, ServiceClosed(handler));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection<D>::ServiceClosed(winrt::event_token const& token) const noexcept
    {
        WINRT_VERIFY_(0, WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection)->remove_ServiceClosed(impl::bind_in(token)));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection2<D>::OpenRemoteAsync(Windows::System::RemoteSystems::RemoteSystemConnectionRequest const& remoteSystemConnectionRequest) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection2)->OpenRemoteAsync(*(void**)(&remoteSystemConnectionRequest), &operation));
        return Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceConnectionStatus>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection2<D>::User() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection2)->get_User(&value));
        return Windows::System::User{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnection2<D>::User(Windows::System::User const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnection2)->put_User(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceConnectionStatics<D>::SendStatelessMessageAsync(Windows::ApplicationModel::AppService::AppServiceConnection const& connection, Windows::System::RemoteSystems::RemoteSystemConnectionRequest const& connectionRequest, Windows::Foundation::Collections::ValueSet const& message) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceConnectionStatics)->SendStatelessMessageAsync(*(void**)(&connection), *(void**)(&connectionRequest), *(void**)(&message), &operation));
        return Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::StatelessAppServiceResponse>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceDeferral<D>::Complete() const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceDeferral)->Complete());
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceRequest<D>::Message() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceRequest)->get_Message(&value));
        return Windows::Foundation::Collections::ValueSet{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceRequest<D>::SendResponseAsync(Windows::Foundation::Collections::ValueSet const& message) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceRequest)->SendResponseAsync(*(void**)(&message), &operation));
        return Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceResponseStatus>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceRequestReceivedEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceRequestReceivedEventArgs)->get_Request(&value));
        return Windows::ApplicationModel::AppService::AppServiceRequest{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceRequestReceivedEventArgs<D>::GetDeferral() const
    {
        void* result{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceRequestReceivedEventArgs)->GetDeferral(&result));
        return Windows::ApplicationModel::AppService::AppServiceDeferral{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceResponse<D>::Message() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceResponse)->get_Message(&value));
        return Windows::Foundation::Collections::ValueSet{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceResponse<D>::Status() const
    {
        Windows::ApplicationModel::AppService::AppServiceResponseStatus value;
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceResponse)->get_Status(put_abi(value)));
        return value;
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceTriggerDetails<D>::Name() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceTriggerDetails)->get_Name(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceTriggerDetails<D>::CallerPackageFamilyName() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceTriggerDetails)->get_CallerPackageFamilyName(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceTriggerDetails<D>::AppServiceConnection() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceTriggerDetails)->get_AppServiceConnection(&value));
        return Windows::ApplicationModel::AppService::AppServiceConnection{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceTriggerDetails2<D>::IsRemoteSystemConnection() const
    {
        bool value;
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceTriggerDetails2)->get_IsRemoteSystemConnection(&value));
        return value;
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceTriggerDetails3<D>::CheckCallerForCapabilityAsync(param::hstring const& capabilityName) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceTriggerDetails3)->CheckCallerForCapabilityAsync(*(void**)(&capabilityName), &operation));
        return Windows::Foundation::IAsyncOperation<bool>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IAppServiceTriggerDetails4<D>::CallerRemoteConnectionToken() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IAppServiceTriggerDetails4)->get_CallerRemoteConnectionToken(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IStatelessAppServiceResponse<D>::Message() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IStatelessAppServiceResponse)->get_Message(&value));
        return Windows::Foundation::Collections::ValueSet{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_ApplicationModel_AppService_IStatelessAppServiceResponse<D>::Status() const
    {
        Windows::ApplicationModel::AppService::StatelessAppServiceResponseStatus value;
        check_hresult(WINRT_IMPL_SHIM(Windows::ApplicationModel::AppService::IStatelessAppServiceResponse)->get_Status(put_abi(value)));
        return value;
    }
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceCatalogStatics> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceCatalogStatics>
    {
        int32_t __stdcall FindAppServiceProvidersAsync(void* appServiceName, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Windows::ApplicationModel::AppInfo>>>(this->shim().FindAppServiceProvidersAsync(*reinterpret_cast<hstring const*>(&appServiceName)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceClosedEventArgs> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceClosedEventArgs>
    {
        int32_t __stdcall get_Status(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::ApplicationModel::AppService::AppServiceClosedStatus>(this->shim().Status());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceConnection> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceConnection>
    {
        int32_t __stdcall get_AppServiceName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppServiceName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_AppServiceName(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().AppServiceName(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_PackageFamilyName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().PackageFamilyName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_PackageFamilyName(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().PackageFamilyName(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall OpenAsync(void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceConnectionStatus>>(this->shim().OpenAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SendMessageAsync(void* message, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceResponse>>(this->shim().SendMessageAsync(*reinterpret_cast<Windows::Foundation::Collections::ValueSet const*>(&message)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall add_RequestReceived(void* handler, winrt::event_token* token) noexcept final try
        {
            zero_abi<winrt::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<winrt::event_token>(this->shim().RequestReceived(*reinterpret_cast<Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::AppService::AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_RequestReceived(winrt::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().RequestReceived(*reinterpret_cast<winrt::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_ServiceClosed(void* handler, winrt::event_token* token) noexcept final try
        {
            zero_abi<winrt::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<winrt::event_token>(this->shim().ServiceClosed(*reinterpret_cast<Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::AppService::AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceClosedEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_ServiceClosed(winrt::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ServiceClosed(*reinterpret_cast<winrt::event_token const*>(&token));
            return 0;
        }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceConnection2> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceConnection2>
    {
        int32_t __stdcall OpenRemoteAsync(void* remoteSystemConnectionRequest, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceConnectionStatus>>(this->shim().OpenRemoteAsync(*reinterpret_cast<Windows::System::RemoteSystems::RemoteSystemConnectionRequest const*>(&remoteSystemConnectionRequest)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_User(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::System::User>(this->shim().User());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_User(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().User(*reinterpret_cast<Windows::System::User const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceConnectionStatics> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceConnectionStatics>
    {
        int32_t __stdcall SendStatelessMessageAsync(void* connection, void* connectionRequest, void* message, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::StatelessAppServiceResponse>>(this->shim().SendStatelessMessageAsync(*reinterpret_cast<Windows::ApplicationModel::AppService::AppServiceConnection const*>(&connection), *reinterpret_cast<Windows::System::RemoteSystems::RemoteSystemConnectionRequest const*>(&connectionRequest), *reinterpret_cast<Windows::Foundation::Collections::ValueSet const*>(&message)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceDeferral> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceDeferral>
    {
        int32_t __stdcall Complete() noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Complete();
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceRequest> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceRequest>
    {
        int32_t __stdcall get_Message(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Foundation::Collections::ValueSet>(this->shim().Message());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SendResponseAsync(void* message, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::ApplicationModel::AppService::AppServiceResponseStatus>>(this->shim().SendResponseAsync(*reinterpret_cast<Windows::Foundation::Collections::ValueSet const*>(&message)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceRequestReceivedEventArgs> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceRequestReceivedEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::ApplicationModel::AppService::AppServiceRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<Windows::ApplicationModel::AppService::AppServiceDeferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceResponse> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceResponse>
    {
        int32_t __stdcall get_Message(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Foundation::Collections::ValueSet>(this->shim().Message());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Status(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::ApplicationModel::AppService::AppServiceResponseStatus>(this->shim().Status());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails>
    {
        int32_t __stdcall get_Name(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Name());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_CallerPackageFamilyName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().CallerPackageFamilyName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppServiceConnection(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::ApplicationModel::AppService::AppServiceConnection>(this->shim().AppServiceConnection());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails2> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails2>
    {
        int32_t __stdcall get_IsRemoteSystemConnection(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsRemoteSystemConnection());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails3> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails3>
    {
        int32_t __stdcall CheckCallerForCapabilityAsync(void* capabilityName, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<bool>>(this->shim().CheckCallerForCapabilityAsync(*reinterpret_cast<hstring const*>(&capabilityName)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails4> : produce_base<D, Windows::ApplicationModel::AppService::IAppServiceTriggerDetails4>
    {
        int32_t __stdcall get_CallerRemoteConnectionToken(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().CallerRemoteConnectionToken());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::ApplicationModel::AppService::IStatelessAppServiceResponse> : produce_base<D, Windows::ApplicationModel::AppService::IStatelessAppServiceResponse>
    {
        int32_t __stdcall get_Message(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Foundation::Collections::ValueSet>(this->shim().Message());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Status(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::ApplicationModel::AppService::StatelessAppServiceResponseStatus>(this->shim().Status());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::AppServiceClosedEventArgs, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::AppServiceClosedEventArgs const& object, hstring const& name)
        {
            if (name == L"Status") return
            {
                [object] { return object.Status(); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::AppServiceConnection, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::AppServiceConnection const& object, hstring const& name)
        {
            if (name == L"AppServiceName") return
            {
                [object] { return object.AppServiceName(); },
                [object](auto&& value) { object.AppServiceName(value); }
            };
            if (name == L"PackageFamilyName") return
            {
                [object] { return object.PackageFamilyName(); },
                [object](auto&& value) { object.PackageFamilyName(value); }
            };
            if (name == L"User") return
            {
                [object] { return object.User(); },
                [object](auto&& value) { object.User(value); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::AppServiceRequest, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::AppServiceRequest const& object, hstring const& name)
        {
            if (name == L"Message") return
            {
                [object] { return object.Message(); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs const& object, hstring const& name)
        {
            if (name == L"Request") return
            {
                [object] { return object.Request(); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::AppServiceResponse, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::AppServiceResponse const& object, hstring const& name)
        {
            if (name == L"Message") return
            {
                [object] { return object.Message(); }
            };
            if (name == L"Status") return
            {
                [object] { return object.Status(); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::AppServiceTriggerDetails, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::AppServiceTriggerDetails const& object, hstring const& name)
        {
            if (name == L"Name") return
            {
                [object] { return object.Name(); }
            };
            if (name == L"CallerPackageFamilyName") return
            {
                [object] { return object.CallerPackageFamilyName(); }
            };
            if (name == L"AppServiceConnection") return
            {
                [object] { return object.AppServiceConnection(); }
            };
            if (name == L"IsRemoteSystemConnection") return
            {
                [object] { return object.IsRemoteSystemConnection(); }
            };
            if (name == L"CallerRemoteConnectionToken") return
            {
                [object] { return object.CallerRemoteConnectionToken(); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::ApplicationModel::AppService::StatelessAppServiceResponse, U>
    {
        static xaml_binding bind(Windows::ApplicationModel::AppService::StatelessAppServiceResponse const& object, hstring const& name)
        {
            if (name == L"Message") return
            {
                [object] { return object.Message(); }
            };
            if (name == L"Status") return
            {
                [object] { return object.Status(); }
            };
            return {};
        }
    };
}
namespace winrt::Windows::ApplicationModel::AppService
{
    inline auto AppServiceCatalog::FindAppServiceProvidersAsync(param::hstring const& appServiceName)
    {
        return impl::call_factory<AppServiceCatalog, Windows::ApplicationModel::AppService::IAppServiceCatalogStatics>([&](auto&& f) { return f.FindAppServiceProvidersAsync(appServiceName); });
    }
    inline AppServiceConnection::AppServiceConnection() :
        AppServiceConnection(impl::call_factory<AppServiceConnection>([](auto&& f) { return f.template ActivateInstance<AppServiceConnection>(); }))
    {
    }
    inline auto AppServiceConnection::SendStatelessMessageAsync(Windows::ApplicationModel::AppService::AppServiceConnection const& connection, Windows::System::RemoteSystems::RemoteSystemConnectionRequest const& connectionRequest, Windows::Foundation::Collections::ValueSet const& message)
    {
        return impl::call_factory<AppServiceConnection, Windows::ApplicationModel::AppService::IAppServiceConnectionStatics>([&](auto&& f) { return f.SendStatelessMessageAsync(connection, connectionRequest, message); });
    }
}
namespace std
{
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceCatalogStatics> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceCatalogStatics> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceClosedEventArgs> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceClosedEventArgs> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceConnection> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceConnection> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceConnection2> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceConnection2> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceConnectionStatics> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceConnectionStatics> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceDeferral> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceDeferral> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceRequest> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceRequest> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceRequestReceivedEventArgs> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceRequestReceivedEventArgs> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceResponse> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceResponse> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails2> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails2> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails3> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails3> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails4> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IAppServiceTriggerDetails4> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::IStatelessAppServiceResponse> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::IStatelessAppServiceResponse> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceCatalog> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceCatalog> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceClosedEventArgs> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceClosedEventArgs> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceConnection> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceConnection> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceDeferral> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceDeferral> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceRequest> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceRequest> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceResponse> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceResponse> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::AppServiceTriggerDetails> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::AppServiceTriggerDetails> {};
    template<> struct hash<winrt::Windows::ApplicationModel::AppService::StatelessAppServiceResponse> : winrt::impl::hash_base<winrt::Windows::ApplicationModel::AppService::StatelessAppServiceResponse> {};
}
#endif
