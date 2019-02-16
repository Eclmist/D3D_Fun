#include "Window.h"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    try
    {
        Window wnd(800, 480, "Fun.exe");

        MSG msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            // TranslateMessage will post auxiliary WM_CHAR messages from key msgs
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // check if GetMessage call itself borked
        if (gResult == -1)
        {
            return -1;
        }

        // wParam here is the value passed to PostQuitMessage
        return msg.wParam;
    }
    catch (const EclException& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "Someone threw an exception wrong!", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}