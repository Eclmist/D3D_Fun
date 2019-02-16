#pragma once
#include "Window.h"
#include "EclTimer.h"

class EclApp
{
public:
    EclApp();
    int start();
private:
    void update();
private:
    Window m_wnd;
    EclTimer m_timer;
};

