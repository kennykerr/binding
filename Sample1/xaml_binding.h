#pragma once

namespace winrt
{
    template <typename D, template <typename...> typename B, typename... I>
    struct xaml_binding : B<D, Windows::UI::Xaml::Data::ICustomPropertyProvider, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
    {
        xaml_binding()
        {
            // TODO: circular reference?
            this->DataContext(*this);
        }

        Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name)
        {
            return static_cast<D*>(this)->bind(name);
        }

        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_changed.add(handler);
        }

        void PropertyChanged(winrt::event_token token)
        {
            m_changed.remove(token);
        }

        Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const&, Windows::UI::Xaml::Interop::TypeName const&) const noexcept { return {}; }
        hstring GetStringRepresentation() const noexcept { return {}; }
        Windows::UI::Xaml::Interop::TypeName Type() const noexcept { return {}; }

    protected:

        void load_markup(hstring const& uri)
        {
            Windows::UI::Xaml::Application::LoadComponent(*this, Uri(uri));
        }

        void property_changed(hstring const& name)
        {
            m_changed(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
        }

        template <typename T>
        auto bind_property(T const& value)
        {
            return make<property<T>>(value);
        }

    private:

        template <typename T>
        struct property : implements<property<T>, Windows::UI::Xaml::Data::ICustomProperty>
        {
            T const& m_value;
            property(T const& value) : m_value(value) {}

            Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const&) const
            {
                return box_value(m_value);
            }

            Windows::UI::Xaml::Interop::TypeName Type() const noexcept { return {}; }
            hstring Name() const noexcept { return {}; }
            void SetValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept {}
            Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept { return {}; }
            void SetIndexedValue(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&) const noexcept {}
            bool CanWrite() const noexcept { return {}; }
            bool CanRead() const noexcept { return {}; }
        };

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_changed;
    };
}