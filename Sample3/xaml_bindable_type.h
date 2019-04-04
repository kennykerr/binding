#pragma once

namespace winrt
{
    // TODO : need a xaml_type that doesn't implement INotifyPropertyChanged?
    template <typename D, template <typename...> typename B, typename... I>
    struct xaml_bindable_type : B<D, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
    {
        // INotifyPropertyChanged
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler) { return m_changed.add(handler); }
        void PropertyChanged(winrt::event_token token) { m_changed.remove(token); }

    protected:

        void InitializeComponent(hstring const& uri) { Windows::UI::Xaml::Application::LoadComponent(*this, Uri(uri)); }
        void property_changed(hstring const& name) { m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name)); }

    private:

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };
}