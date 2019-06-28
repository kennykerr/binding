// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.3.4.5

#ifndef WINRT_Windows_Media_ContentRestrictions_H
#define WINRT_Windows_Media_ContentRestrictions_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.3.4.5"), "Mismatched C++/WinRT headers.");
#include "winrt/Windows.Media.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Foundation.Collections.2.h"
#include "winrt/impl/Windows.Storage.Streams.2.h"
#include "winrt/impl/Windows.Media.ContentRestrictions.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IContentRestrictionsBrowsePolicy<D>::GeographicRegion() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy)->get_GeographicRegion(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IContentRestrictionsBrowsePolicy<D>::MaxBrowsableAgeRating() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy)->get_MaxBrowsableAgeRating(&value));
        return Windows::Foundation::IReference<uint32_t>{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IContentRestrictionsBrowsePolicy<D>::PreferredAgeRating() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy)->get_PreferredAgeRating(&value));
        return Windows::Foundation::IReference<uint32_t>{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Id() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->get_Id(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Id(param::hstring const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->put_Id(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Title() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->get_Title(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Title(param::hstring const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->put_Title(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Image() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->get_Image(&value));
        return Windows::Storage::Streams::IRandomAccessStreamReference{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Image(Windows::Storage::Streams::IRandomAccessStreamReference const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->put_Image(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Category() const
    {
        Windows::Media::ContentRestrictions::RatedContentCategory value;
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->get_Category(put_abi(value)));
        return value;
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Category(Windows::Media::ContentRestrictions::RatedContentCategory const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->put_Category(static_cast<int32_t>(value)));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Ratings() const
    {
        void* value{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->get_Ratings(&value));
        return Windows::Foundation::Collections::IVector<hstring>{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescription<D>::Ratings(param::vector<hstring> const& value) const
    {
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescription)->put_Ratings(*(void**)(&value)));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentDescriptionFactory<D>::Create(param::hstring const& id, param::hstring const& title, Windows::Media::ContentRestrictions::RatedContentCategory const& category) const
    {
        void* RatedContentDescription{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentDescriptionFactory)->Create(*(void**)(&id), *(void**)(&title), static_cast<int32_t>(category), &RatedContentDescription));
        return Windows::Media::ContentRestrictions::RatedContentDescription{ RatedContentDescription, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::GetBrowsePolicyAsync() const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentRestrictions)->GetBrowsePolicyAsync(&operation));
        return Windows::Foundation::IAsyncOperation<Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::GetRestrictionLevelAsync(Windows::Media::ContentRestrictions::RatedContentDescription const& RatedContentDescription) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentRestrictions)->GetRestrictionLevelAsync(*(void**)(&RatedContentDescription), &operation));
        return Windows::Foundation::IAsyncOperation<Windows::Media::ContentRestrictions::ContentAccessRestrictionLevel>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::RequestContentAccessAsync(Windows::Media::ContentRestrictions::RatedContentDescription const& RatedContentDescription) const
    {
        void* operation{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentRestrictions)->RequestContentAccessAsync(*(void**)(&RatedContentDescription), &operation));
        return Windows::Foundation::IAsyncOperation<bool>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::RestrictionsChanged(Windows::Foundation::EventHandler<Windows::Foundation::IInspectable> const& handler) const
    {
        winrt::event_token token;
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentRestrictions)->add_RestrictionsChanged(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::RestrictionsChanged_revoker consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::RestrictionsChanged(auto_revoke_t, Windows::Foundation::EventHandler<Windows::Foundation::IInspectable> const& handler) const
    {
        return impl::make_event_revoker<D, RestrictionsChanged_revoker>(this, RestrictionsChanged(handler));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentRestrictions<D>::RestrictionsChanged(winrt::event_token const& token) const noexcept
    {
        WINRT_VERIFY_(0, WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentRestrictions)->remove_RestrictionsChanged(impl::bind_in(token)));
    }
    template <typename D> auto consume_Windows_Media_ContentRestrictions_IRatedContentRestrictionsFactory<D>::CreateWithMaxAgeRating(uint32_t maxAgeRating) const
    {
        void* ratedContentRestrictions{};
        check_hresult(WINRT_IMPL_SHIM(Windows::Media::ContentRestrictions::IRatedContentRestrictionsFactory)->CreateWithMaxAgeRating(maxAgeRating, &ratedContentRestrictions));
        return Windows::Media::ContentRestrictions::RatedContentRestrictions{ ratedContentRestrictions, take_ownership_from_abi };
    }
    template <typename D>
    struct produce<D, Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy> : produce_base<D, Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy>
    {
        int32_t __stdcall get_GeographicRegion(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().GeographicRegion());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_MaxBrowsableAgeRating(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Foundation::IReference<uint32_t>>(this->shim().MaxBrowsableAgeRating());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_PreferredAgeRating(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Foundation::IReference<uint32_t>>(this->shim().PreferredAgeRating());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::Media::ContentRestrictions::IRatedContentDescription> : produce_base<D, Windows::Media::ContentRestrictions::IRatedContentDescription>
    {
        int32_t __stdcall get_Id(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Id(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Id(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Title(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Title());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Title(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Title(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Image(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Storage::Streams::IRandomAccessStreamReference>(this->shim().Image());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Image(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Image(*reinterpret_cast<Windows::Storage::Streams::IRandomAccessStreamReference const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Category(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Media::ContentRestrictions::RatedContentCategory>(this->shim().Category());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Category(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Category(*reinterpret_cast<Windows::Media::ContentRestrictions::RatedContentCategory const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Ratings(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<Windows::Foundation::Collections::IVector<hstring>>(this->shim().Ratings());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Ratings(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Ratings(*reinterpret_cast<Windows::Foundation::Collections::IVector<hstring> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::Media::ContentRestrictions::IRatedContentDescriptionFactory> : produce_base<D, Windows::Media::ContentRestrictions::IRatedContentDescriptionFactory>
    {
        int32_t __stdcall Create(void* id, void* title, int32_t category, void** RatedContentDescription) noexcept final try
        {
            clear_abi(RatedContentDescription);
            typename D::abi_guard guard(this->shim());
            *RatedContentDescription = detach_from<Windows::Media::ContentRestrictions::RatedContentDescription>(this->shim().Create(*reinterpret_cast<hstring const*>(&id), *reinterpret_cast<hstring const*>(&title), *reinterpret_cast<Windows::Media::ContentRestrictions::RatedContentCategory const*>(&category)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, Windows::Media::ContentRestrictions::IRatedContentRestrictions> : produce_base<D, Windows::Media::ContentRestrictions::IRatedContentRestrictions>
    {
        int32_t __stdcall GetBrowsePolicyAsync(void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy>>(this->shim().GetBrowsePolicyAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetRestrictionLevelAsync(void* RatedContentDescription, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<Windows::Media::ContentRestrictions::ContentAccessRestrictionLevel>>(this->shim().GetRestrictionLevelAsync(*reinterpret_cast<Windows::Media::ContentRestrictions::RatedContentDescription const*>(&RatedContentDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall RequestContentAccessAsync(void* RatedContentDescription, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<Windows::Foundation::IAsyncOperation<bool>>(this->shim().RequestContentAccessAsync(*reinterpret_cast<Windows::Media::ContentRestrictions::RatedContentDescription const*>(&RatedContentDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall add_RestrictionsChanged(void* handler, winrt::event_token* token) noexcept final try
        {
            zero_abi<winrt::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<winrt::event_token>(this->shim().RestrictionsChanged(*reinterpret_cast<Windows::Foundation::EventHandler<Windows::Foundation::IInspectable> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_RestrictionsChanged(winrt::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().RestrictionsChanged(*reinterpret_cast<winrt::event_token const*>(&token));
            return 0;
        }
    };
    template <typename D>
    struct produce<D, Windows::Media::ContentRestrictions::IRatedContentRestrictionsFactory> : produce_base<D, Windows::Media::ContentRestrictions::IRatedContentRestrictionsFactory>
    {
        int32_t __stdcall CreateWithMaxAgeRating(uint32_t maxAgeRating, void** ratedContentRestrictions) noexcept final try
        {
            clear_abi(ratedContentRestrictions);
            typename D::abi_guard guard(this->shim());
            *ratedContentRestrictions = detach_from<Windows::Media::ContentRestrictions::RatedContentRestrictions>(this->shim().CreateWithMaxAgeRating(maxAgeRating));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename U> struct bindable<Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy, U>
    {
        static xaml_binding bind(Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy const& object, hstring const& name)
        {
            if (name == L"GeographicRegion") return
            {
                [object] { return object.GeographicRegion(); }
            };
            if (name == L"MaxBrowsableAgeRating") return
            {
                [object] { return object.MaxBrowsableAgeRating(); }
            };
            if (name == L"PreferredAgeRating") return
            {
                [object] { return object.PreferredAgeRating(); }
            };
            return {};
        }
    };
    template <typename U> struct bindable<Windows::Media::ContentRestrictions::RatedContentDescription, U>
    {
        static xaml_binding bind(Windows::Media::ContentRestrictions::RatedContentDescription const& object, hstring const& name)
        {
            if (name == L"Id") return
            {
                [object] { return object.Id(); },
                [object](auto&& value) { object.Id(value); }
            };
            if (name == L"Title") return
            {
                [object] { return object.Title(); },
                [object](auto&& value) { object.Title(value); }
            };
            if (name == L"Image") return
            {
                [object] { return object.Image(); },
                [object](auto&& value) { object.Image(value); }
            };
            if (name == L"Category") return
            {
                [object] { return object.Category(); },
                [object](auto&& value) { object.Category(value); }
            };
            if (name == L"Ratings") return
            {
                [object] { return object.Ratings(); },
                [object](auto&& value) { object.Ratings(value); }
            };
            return {};
        }
    };
}
namespace winrt::Windows::Media::ContentRestrictions
{
    inline RatedContentDescription::RatedContentDescription(param::hstring const& id, param::hstring const& title, Windows::Media::ContentRestrictions::RatedContentCategory const& category) :
        RatedContentDescription(impl::call_factory<RatedContentDescription, Windows::Media::ContentRestrictions::IRatedContentDescriptionFactory>([&](auto&& f) { return f.Create(id, title, category); }))
    {
    }
    inline RatedContentRestrictions::RatedContentRestrictions() :
        RatedContentRestrictions(impl::call_factory<RatedContentRestrictions>([](auto&& f) { return f.template ActivateInstance<RatedContentRestrictions>(); }))
    {
    }
    inline RatedContentRestrictions::RatedContentRestrictions(uint32_t maxAgeRating) :
        RatedContentRestrictions(impl::call_factory<RatedContentRestrictions, Windows::Media::ContentRestrictions::IRatedContentRestrictionsFactory>([&](auto&& f) { return f.CreateWithMaxAgeRating(maxAgeRating); }))
    {
    }
}
namespace std
{
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::IContentRestrictionsBrowsePolicy> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::IRatedContentDescription> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::IRatedContentDescription> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::IRatedContentDescriptionFactory> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::IRatedContentDescriptionFactory> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::IRatedContentRestrictions> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::IRatedContentRestrictions> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::IRatedContentRestrictionsFactory> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::IRatedContentRestrictionsFactory> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::RatedContentDescription> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::RatedContentDescription> {};
    template<> struct hash<winrt::Windows::Media::ContentRestrictions::RatedContentRestrictions> : winrt::impl::hash_base<winrt::Windows::Media::ContentRestrictions::RatedContentRestrictions> {};
}
#endif
