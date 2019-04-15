#pragma once

#include "DesktopWindow.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

struct XamlIsland : DesktopWindow<XamlIsland>
{
    XamlIsland() noexcept
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

    ~XamlIsland()
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

    void DoResize(UINT width, UINT height)
    {
        OnSize(m_interopWindow, m_rootGrid, width, height);
    }

    LRESULT MessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
    {
        // TODO: handle messages here...
        return base_type::MessageHandler(message, wparam, lparam);
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

        auto scaleFactor = (float)m_currentDpi / 100;

        if (rootGrid != nullptr) {
            rootGrid.Width(physicalWidth / scaleFactor);
            rootGrid.Height(physicalHeight / scaleFactor);
        }
    }

    UINT m_currentWidthPhysical{ 800 };
    UINT m_currentHeightPhysical{ 600 };
    HWND m_interopWindow{ nullptr };
    winrt::Windows::UI::Xaml::UIElement m_content{ nullptr };
    winrt::Windows::UI::Xaml::Controls::Grid m_rootGrid{ nullptr };
    winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource m_xamlSource{ nullptr };
    winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager m_xamlManager{ nullptr };
};