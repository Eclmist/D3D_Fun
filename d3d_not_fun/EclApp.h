#pragma once
#include "Window.h"
#include "EclTimer.h"

class EclApp
{
public:
    EclApp();
    int Start();
private:
    void Update();
private:
    Window wnd;
    EclTimer timer;
};

