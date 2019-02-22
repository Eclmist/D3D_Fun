#include "window.h"
#include "resource.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
    // Registers window class properties
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

Window::Window(int m_Width, int m_Height, const char* name) : m_Width(m_Width), m_Height(m_Height)
{
    RECT wr;
    wr.left = 0;
    wr.right = m_Width;
    wr.top = 0;
    wr.bottom = m_Height;

    // Ensure that width and height is only inclusive of client region
    HRESULT hr = AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU, false);

    if (FAILED(hr)) throw CHWND_LAST_EXCEPT();

    // Create window and get HWND
    m_hWnd = CreateWindow(
        WindowClass::GetName(),
        name,
        WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );

    if (m_hWnd == nullptr) throw CHWND_LAST_EXCEPT();

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);

    m_pGraphics = std::make_unique<Graphics>(m_hWnd);
}

Window::~Window()
{
    DestroyWindow(m_hWnd);
}

void Window::SetTitle(const std::string & title)
{
    if (SetWindowText(m_hWnd, title.c_str()) == 0)
    {
        throw CHWND_LAST_EXCEPT();
    }
}

std::optional<int> Window::ProcessMessages()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

Graphics & Window::Gfx()
{
    return *m_pGraphics;
}

/**
 * Basically allows us to use a custom function for handling of windows messages
 * by sneaking a pointer to our Window class into lParams and extracting it when
 * the window is created
 */
LRESULT CALLBACK Window::HandleMsgSetup(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window class handler
        return pWnd->HandleMsg(m_hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(m_hWnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->HandleMsg(m_hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        // we don't want the DefProc to handle this message because
        // we want our destructor to destroy the window, so return 0 instead of break
        return 0;
    case WM_KILLFOCUS:
        m_Kbd.ClearState();
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        // Implicitly disable auto repeat
        if (!(lParam & 0x40000000) || m_Kbd.AutorepeatIsEnabled())
            m_Kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        m_Kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        m_Kbd.OnChar(static_cast<unsigned char>(wParam));
        break;
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        // In client region
        if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
        {
            m_Mouse.OnMouseMove(pt.x, pt.y);

            if (!m_Mouse.IsInWindow())
            {
                SetCapture(m_hWnd);
                m_Mouse.OnMouseEnter();
            }
        }
        else 
        {
            if ((wParam & (MK_LBUTTON | MK_RBUTTON)) != 0) 
            {
                POINTS ptClamped = pt;
                ptClamped.x = std::max(0, std::min(static_cast<int>(ptClamped.x), m_Width));
                ptClamped.y = std::max(0, std::min(static_cast<int>(ptClamped.y), m_Width));
                m_Mouse.OnMouseMove(ptClamped.x, ptClamped.y);
            }
            else
            {
                ReleaseCapture();
                m_Mouse.OnMouseLeave();
            }
        }

        break;
    }
    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        m_Mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        m_Mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        m_Mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        m_Mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        m_Mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }
    }

    return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

Window::Exception::Exception(int line, const char * file, HRESULT hr) noexcept : EclException(line, file), hr(hr) { }

const char * Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
    return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );
    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr);
}
