#include "EclApp.h"
#include <sstream>
#include <iomanip>

EclApp::EclApp() : wnd(800, 480, "EclApp") {}

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
