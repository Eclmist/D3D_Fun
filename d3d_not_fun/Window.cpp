#include "Window.h"
#include "resource.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
    // Registers window class properties
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = getInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = getName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, getInstance());
}

const char* Window::WindowClass::getName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept
{
    return wndClass.hInst;
}

Window::Window(int width, int height, const char* name) : m_width(width), m_height(height)
{
    RECT wr;
    wr.left = 0;
    wr.right = width;
    wr.top = 0;
    wr.bottom = height;

    // Ensure that width and height is only inclusive of client region
    HRESULT hr = AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU, false);

    if (FAILED(hr)) throw CHWND_LAST_EXCEPT();

    // Create window and get HWND
    m_hWnd = CreateWindow(
        WindowClass::getName(),
        name,
        WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::getInstance(),
        this
    );

    if (m_hWnd == nullptr) throw CHWND_LAST_EXCEPT();

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(m_hWnd);
}

void Window::setTitle(const std::string & title)
{
    if (SetWindowText(m_hWnd, title.c_str()) == 0)
    {
        throw CHWND_LAST_EXCEPT();
    }
}

std::optional<int> Window::processMessages()
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

/**
 * Basically allows us to use a custom function for handling of windows messages
 * by sneaking a pointer to our Window class into lParams and extracting it when
 * the window is created
 */
LRESULT CALLBACK Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgThunk));
        // forward message to window class handler
        return pWnd->handleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        // we don't want the DefProc to handle this message because
        // we want our destructor to destroy the window, so return 0 instead of break
        return 0;
    case WM_KILLFOCUS:
        kbd.clearState();
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        // Implicitly disable auto repeat
        if (!(lParam & 0x40000000) || kbd.autorepeatIsEnabled())
            kbd.onKeyPressed(static_cast<unsigned char>(wParam));
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        kbd.onKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        kbd.onChar(static_cast<unsigned char>(wParam));
        break;
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        // In client region
        if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
        {
            mouse.onMouseMove(pt.x, pt.y);

            if (!mouse.isInWindow())
            {
                SetCapture(hWnd);
                mouse.onMouseEnter();
            }
        }
        else 
        {
            if ((wParam & (MK_LBUTTON | MK_RBUTTON)) != 0) 
            {
                POINTS ptClamped = pt;
                ptClamped.x = std::max(0, std::min(static_cast<int>(ptClamped.x), m_width));
                ptClamped.y = std::max(0, std::min(static_cast<int>(ptClamped.y), m_width));
                mouse.onMouseMove(ptClamped.x, ptClamped.y);
            }
            else
            {
                ReleaseCapture();
                mouse.onMouseLeave();
            }
        }

        break;
    }
    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.onRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.onWheelDelta(pt.x, pt.y, delta);
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Exception::Exception(int line, const char * file, HRESULT hr) noexcept : EclException(line, file), hr(hr) { }

const char * Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Error Code] " << getErrorCode() << std::endl
        << "[Description] " << getErrorString() << std::endl
        << getOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::Exception::getType() const noexcept
{
    return "Window Exception";
}

std::string Window::Exception::translateErrorCode(HRESULT hr) noexcept
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

HRESULT Window::Exception::getErrorCode() const noexcept
{
    return hr;
}

std::string Window::Exception::getErrorString() const noexcept
{
    return translateErrorCode(hr);
}
