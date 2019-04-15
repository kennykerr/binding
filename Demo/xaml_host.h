#pragma once

extern "C" IMAGE_DOS_HEADER __ImageBase;

struct XamlHost 
{
    XamlHost() noexcept
    {
        WNDCLASS wc{};
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
        wc.lpszClassName = L"Desktop XAML with C++/WinRT";
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        RegisterClass(&wc);
        WINRT_ASSERT(!m_window);

        WINRT_VERIFY(CreateWindow(wc.lpszClassName,
            L"Desktop XAML with C++/WinRT",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr, nullptr, wc.hInstance, this));

        WINRT_ASSERT(m_window);

        InitXaml();
    }

    ~XamlHost()
    {
        m_xamlManager.Close();
    }

    void Run()
    {
        MSG message;
        while (GetMessage(&message, nullptr, 0, 0))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

    void Content(winrt::Windows::UI::Xaml::UIElement content)
    {
        m_rootGrid.Children().Clear();
        m_rootGrid.Children().Append(content);
    }

private:
    void InitXaml()
    {
        m_xamlManager = winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager::InitializeForCurrentThread();

        m_xamlSource = winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource();
        auto interop = m_xamlSource.as<IDesktopWindowXamlSourceNative>();
        winrt::check_hresult(interop->AttachToWindow((HWND)m_window));
        interop->get_WindowHandle(&m_interopWindow);

        m_xamlSource.Content(m_rootGrid = winrt::Windows::UI::Xaml::Controls::Grid());

        OnSize(m_interopWindow, m_rootGrid, m_currentWidthPhysical, m_currentHeightPhysical);
    }

    void OnSize(HWND interopHandle,
        winrt::Windows::UI::Xaml::Controls::Grid & rootGrid,
        UINT physicalWidth,
        UINT physicalHeight)
    {
        SetWindowPos(interopHandle, 0, 0, 0, physicalWidth, physicalHeight, SWP_SHOWWINDOW);

        m_currentWidthPhysical = physicalWidth;
        m_currentHeightPhysical = physicalHeight;

        if (rootGrid != nullptr) {
            rootGrid.Width(physicalWidth);
            rootGrid.Height(physicalHeight);
        }
    }

    static auto GetThisFromHandle(HWND const window) noexcept
    {
        return reinterpret_cast<XamlHost*>(GetWindowLongPtr(window, GWLP_USERDATA));
    }

    static LRESULT __stdcall WndProc(HWND const window, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
    {
        WINRT_ASSERT(window);

        if (WM_NCCREATE == message)
        {
            auto cs = reinterpret_cast<CREATESTRUCT*>(lparam);
            auto that = static_cast<XamlHost*>(cs->lpCreateParams);
            WINRT_ASSERT(that);
            WINRT_ASSERT(!that->m_window);
            that->m_window = window;
            SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(that));
        }
        else if (auto that = GetThisFromHandle(window))
        {
            return that->MessageHandler(message, wparam, lparam);
        }

        return DefWindowProc(window, message, wparam, lparam);
    }

    LRESULT MessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
    {
        switch (message)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProc(m_window, message, wparam, lparam);
    }

    HWND m_window = nullptr;
    UINT m_currentWidthPhysical{ 400 };
    UINT m_currentHeightPhysical{ 300 };
    HWND m_interopWindow{ nullptr };
    winrt::Windows::UI::Xaml::UIElement m_content{ nullptr };
    winrt::Windows::UI::Xaml::Controls::Grid m_rootGrid{ nullptr };
    winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource m_xamlSource{ nullptr };
    winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager m_xamlManager{ nullptr };
};