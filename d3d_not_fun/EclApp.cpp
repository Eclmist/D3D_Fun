#include "EclApp.h"
#include <sstream>

EclApp::EclApp() : m_wnd(800, 480, "EclApp") {}

int EclApp::start()
{
    while (true)
    {
        if (const auto exitCode = Window::processMessages())
        {
            return *exitCode;
        }

        update();
    }
}

void EclApp::update()
{
}
