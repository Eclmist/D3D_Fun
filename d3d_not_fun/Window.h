#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "eclwin.h"
#include "eclexception.h"
#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include <optional>
#include <memory>

#define CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr) 
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError()) 

class Window
{
public:
    class Exception : public EclException
    {
    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        virtual const char* GetType() const noexcept;
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
    private:
        HRESULT hr;
    };

private:
    class WindowClass
    {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const char* wndClassName = "Eclipse3D";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };

public:
    Window(int m_Width, int m_Height, const char* name);
    ~Window();
    Window(const WindowClass&) = delete;
    Window& operator=(const WindowClass&) = delete;
    void SetTitle(const std::string& title);
    static std::optional<int> ProcessMessages();
    Graphics& Gfx();

public:
    Keyboard m_Kbd;
    Mouse m_Mouse;

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    int m_Width;
    int m_Height;
    HWND m_hWnd;
    std::unique_ptr<Graphics> m_pGraphics;
};

#endif // !__WINDOW_H__
