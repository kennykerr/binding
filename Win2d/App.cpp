#include "pch.h"
#include "xaml.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Microsoft.Graphics.Canvas.UI.Xaml.h"
#include "winuser.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml;
using namespace Windows::UI;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Media::Playback;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Core;
using namespace Windows::UI::Text;
using namespace Windows::UI::Input::Inking;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Media::Core;

#define DESKTOP 1

struct MyPage : xaml_page<MyPage>
{
    CanvasControl m_canvas{ nullptr };
    MyPage() : base_type(L"ms-appx:///MainPage.xaml")
    {
        struct MyControl : Windows::UI::Xaml::Controls::UserControlT<MyControl, inspectable>
        {
            MyControl()
            {
                Grid grid;
                TextBlock tb;
                tb.Text(L"Hello Win32 love XAML and C++/WinRT xx");
                tb.FontSize(30.0f);
                grid.Children().Append(tb);
                Content(grid);
            }
        };

        Content(make<MyControl>());
    }

    void canvasControl_Draw(CanvasControl const& sender, CanvasDrawEventArgs const& args)
    {
        float2 size = sender.Size();
        float2 center{ size.x / 2.0f, size.y / 2.0f };
        Rect bounds{ 0.0f, 0.0f, size.x, size.y };

        CanvasDrawingSession session = args.DrawingSession();

        session.FillEllipse(center, center.x - 50.0f, center.y - 50.0f, Colors::Black());
        session.DrawText(L"Win2D with\nC++/WinRT!", bounds.X, bounds.Y, Colors::Orange());
    }
};

#pragma region Xaml Traits
namespace winrt
{
    template<>
    struct xaml_traits<CanvasControl> : xaml_non_observable<CanvasControl>
    {
        static hstring GetRuntimeClassName()
        {
            return L"Microsoft.Graphics.Canvas.UI.Xaml.CanvasControl";
        }

        static xaml_member bind(CanvasControl& instance, hstring const& name)
        {
            if (name == L"Draw")
            {
                struct accessor : xaml_accessor
                {
                    CanvasControl _instance;

                    accessor(CanvasControl& instance) : _instance(instance) {}

                    inspectable get() override
                    {
                        return {};
                    }

                    void put(inspectable const& value) override
                    {
                        auto handler = unbox_value<winrt::hstring>(value);
                        if (handler == L"canvasControl_Draw")
                        {
                            _instance.Draw({ (_instance.DataContext()).as<MyPage>(), &MyPage::canvasControl_Draw });
                        }
                    }
                };
                return new accessor(instance);
            }
            if (name == L"ClearColor")
            {
                struct accessor : xaml_accessor
                {
                    CanvasControl _instance;

                    accessor(CanvasControl& instance) : _instance(instance) {}

                    inspectable get() override
                    {
                        return box_value(_instance.ClearColor());
                    }

                    void put(inspectable const& value) override
                    {
                        auto color = unbox_value<winrt::hstring>(value);
                        if (color == L"CornflowerBlue")
                        {
                            _instance.ClearColor(Colors::CornflowerBlue());
                        }
                    }
                };
                return new accessor(instance);
            }
            return {};
        }
    };
}
#pragma endregion


#if DESKTOP

template <typename T>
struct DesktopWindow
{
    static T* GetThisFromHandle(HWND const window) noexcept
    {
        return reinterpret_cast<T*>(GetWindowLongPtr(window, GWLP_USERDATA));
    }

    static LRESULT __stdcall WndProc(HWND const window, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
    {
        WINRT_ASSERT(window);

        if (WM_NCCREATE == message)
        {
            auto cs = reinterpret_cast<CREATESTRUCT*>(lparam);
            T* that = static_cast<T*>(cs->lpCreateParams);
            WINRT_ASSERT(that);
            WINRT_ASSERT(!that->m_window);
            that->m_window = window;
            SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(that));
        }
        else if (T * that = GetThisFromHandle(window))
        {
            return that->MessageHandler(message, wparam, lparam);
        }

        return DefWindowProc(window, message, wparam, lparam);
    }

    LRESULT MessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
    {
        if (WM_DESTROY == message)
        {
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(m_window, message, wparam, lparam);
    }

    //protected:

    using base_type = DesktopWindow<T>;
    HWND m_window = nullptr;
};

struct MyWindow : DesktopWindow<MyWindow>
{
    MyWindow() noexcept
    {
        WNDCLASS wc{};
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
        wc.lpszClassName = "Demo";
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        WINRT_VERIFY(RegisterClass(&wc));
        WINRT_ASSERT(!m_window);
        
        WINRT_VERIFY(CreateWindow(wc.lpszClassName,
            "Demo",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr, nullptr, wc.hInstance, this));

        WINRT_ASSERT(m_window);
    }

    LRESULT MessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
    {
        // TODO: handle messages here...

        return base_type::MessageHandler(message, wparam, lparam);
    }

    DesktopWindowXamlSource m_xamlSource{ nullptr };
};

DesktopWindowXamlSource windowXamlSource;


// Defines and registers WNDCLASS structs for windows, creates the windows including the window for hosting the XAML Navigation View control,
// and displays the navigation UI. 
auto CreateXamlIsland(MyWindow& window, int nCmdShow)
{
    //Set windows show state and send WM_PAINT message
    auto hwndMain = window.m_window;
    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);
    RECT appRect;
    GetClientRect(hwndMain, &appRect);

    //Setup XAML island and associate with the child window
    WindowsXamlManager windowsXamlManager = WindowsXamlManager::InitializeForCurrentThread();

    auto interopWindow = windowXamlSource.as<IDesktopWindowXamlSourceNative>();
    check_hresult(interopWindow->AttachToWindow(hwndMain));

    //Stash the interop handle so that we can resize it when the main hwnd is resized
    HWND hwndXamlIsland = nullptr;
    interopWindow->get_WindowHandle(&hwndXamlIsland);

    // create a xaml element
    auto xamlElement = make<MyPage>(); 

    windowXamlSource.Content(xamlElement);
    SetWindowPos(hwndXamlIsland, 0, 0, 0, appRect.right, appRect.bottom, SWP_SHOWWINDOW);

    //End XAML Island
    return xamlElement;
}

#else

struct App : xaml_app<App>
{
    void OnLaunched(LaunchActivatedEventArgs const&)
    {
        auto window = Window::Current();
        window.Content(make<MyPage>());
        window.Activate();
    }
};

#endif

using namespace Windows::Storage;

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nCmdShow)
{
#if DESKTOP
    init_apartment(apartment_type::single_threaded);

    MyWindow window;
    auto xamlIsland = CreateXamlIsland(window, nCmdShow);

    MSG message;
    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
#else
    Application::Start([](auto&&) { make<App>(); });
#endif

}
