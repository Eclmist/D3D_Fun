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
    const float c = sin(m_Timer.Peek()) / 2.0f + 0.5f;
    m_Wnd.Gfx().ClearRenderTarget(c, c, c);
    m_Wnd.Gfx().FlipAndPresentFrame();
}
