#pragma once
#include "EclWin.h"
#include "EclException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>

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
        virtual const char* getType() const noexcept;
        static std::string translateErrorCode(HRESULT hr) noexcept;
        HRESULT getErrorCode() const noexcept;
        std::string getErrorString() const noexcept;
    private:
        HRESULT hr;
    };

private:
    class WindowClass
    {
    public:
        static const char* getName() noexcept;
        static HINSTANCE getInstance() noexcept;
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
    Window(int width, int height, const char* name);
    ~Window();
    Window(const WindowClass&) = delete;
    Window& operator=(const WindowClass&) = delete;
    void SetTitle(const std::string& title);
    static std::optional<int> processMessages();

public:
    Keyboard kbd;
    Mouse mouse;

private:
    static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    int m_width;
    int m_height;
    HWND m_hWnd;
};
