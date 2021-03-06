#include "window.h"
#include "eclapp.h"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    try
    {
        return EclApp{}.Start();
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
