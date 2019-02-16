#pragma once
#include "EclWin.h"
#include "EclException.h"
#include "Keyboard.h"

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
    Window(int width, int height, const char* name);
    ~Window();
    Window(const WindowClass&) = delete;
    Window& operator=(const WindowClass&) = delete;

public:
    Keyboard kbd;

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    int width;
    int height;
    HWND hWnd;
};
