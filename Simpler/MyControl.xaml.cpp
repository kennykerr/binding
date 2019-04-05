#include "pch.h"

#include "MyControl.xaml.h"

#include "XamlTypeRegistry.h"

using namespace std::literals;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Markup;

using namespace Sample2;

static bool s_bMyControlRegistered = XamlTypeRegistry::RegisterType<MyControl>();

hstring MyControl::GetRuntimeClassName()
{
    return L"Sample2.MyControl";
}

XamlTypeRegistry::MemberInformation MyControl::GetMember(hstring const& name)
{
    if (name == L"DisplayText")
    {
        return {
            name,
            XamlTypeRegistry::GetType(L"String"),
            [](IInspectable instance) { return winrt::box_value(instance.as<MyControl>()->get_DisplayText()); },
            nullptr };
    }

    return {};
}

MyControl::MyControl()
{
    DataContext(*this);

    m_loadedEventToken = Loaded({ this, &MyControl::OnLoaded });

    InitializeComponent(L"ms-appx:///MyControl.xaml");
}

hstring MyControl::get_DisplayText()
{
    return m_text;
}

void MyControl::put_DisplayText(hstring const& value)
{
    // TODO : if value has changed ...
    m_text = value;
    property_changed(L"DisplayText");
}

void MyControl::OnLoaded(IInspectable const&, RoutedEventArgs const&)
{
    Loaded(m_loadedEventToken);

    UpdateAsync();
}

IAsyncAction MyControl::UpdateAsync()
{
    apartment_context context;
    int value{};

    while (true)
    {
        co_await 1s;
        co_await context;

        put_DisplayText(hstring(std::to_wstring(++value)));
    }
}