#include "pch.h"
#include "xaml.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;


struct MyViewModelObject : implements<MyViewModelObject, inspectable>
{
	hstring m_text{ L"View Model Object" };
};

namespace winrt
{
	template<>
	struct xaml_traits<MyViewModelObject> : xaml_non_observable_type<MyViewModelObject>
	{
		static hstring GetRuntimeClassName()
		{
			return L"Sample.MyViewModelObject";
		}

		static xaml_member bind(MyViewModelObject* self, hstring const& name)
		{
			if (name == L"Text")
			{
				return self->m_text;
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
	};

	template<>
	struct xaml_traits<xaml_wrapper<MyViewModelStruct>> : xaml_non_observable_type<xaml_wrapper<MyViewModelStruct>>
	{
		static hstring GetRuntimeClassName()
		{
			return L"Sample.MyViewModelStruct";
		}

		static xaml_member bind(xaml_wrapper<MyViewModelStruct>* self, hstring const& name)
		{
			if (name == L"Text")
			{
				return self->m_text;
			}
			return {};
		}
	};
}
