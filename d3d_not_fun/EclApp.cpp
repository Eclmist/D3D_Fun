#include "eclapp.h"
#include <sstream>

EclApp::EclApp() : m_Wnd(1366, 768, "EclApp") {}

int EclApp::Start()
{
    while (true)
    {
        if (const auto exitCode = Window::ProcessMessages())
        {
            return *exitCode;
        }

        Update();
    }
}

void EclApp::Update()
{
}
