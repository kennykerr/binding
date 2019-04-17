#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;


struct MyViewModelObject : implements<MyViewModelObject, inspectable>
{
    hstring m_text{ L"View Model Object" };

    static hstring GetRuntimeClassName()
    {
    	return L"Sample.MyViewModelObject";
    }
};

namespace winrt
{
	template<>
	struct xaml_traits<MyViewModelObject> : xaml_non_observable<MyViewModelObject>
	{
		static hstring GetRuntimeClassName()
		{
            return MyViewModelObject::GetRuntimeClassName();
		}

		static xaml_member bind(MyViewModelObject& instance, hstring const& name)
		{
			if (name == L"Text")
			{
				return instance.m_text;
			}
			return {};
		}
	};
}

struct MyViewModelStruct 
{
	hstring m_text{ L"View Model Struct" };
};

namespace winrt
{
	template<typename T>
	struct xaml_wrapper : implements<xaml_wrapper<T>, inspectable>, T
	{
        static hstring GetRuntimeClassName()
        {
            return L"Sample.MyViewModelStruct";
        }
    };

	template<>
	struct xaml_traits<xaml_wrapper<MyViewModelStruct>> : xaml_non_observable<xaml_wrapper<MyViewModelStruct>>
	{
		static hstring GetRuntimeClassName()
		{
            return xaml_wrapper<MyViewModelStruct>::GetRuntimeClassName();
		}

		static xaml_member bind(xaml_wrapper<MyViewModelStruct>& instance, hstring const& name)
		{
			if (name == L"Text")
			{
				return instance.m_text;
			}
			return {};
		}
	};
}
