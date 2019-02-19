#pragma once
#include "window.h"
#include "ecltimer.h"

class EclApp
{
public:
    EclApp();
    int Start();
private:
    void Update();
private:
    Window m_Wnd;
    EclTimer m_Timer;
};

