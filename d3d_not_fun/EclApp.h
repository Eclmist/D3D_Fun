#ifndef __ECL_APP_H__
#define __ECL_APP_H__
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

#endif // !__ECL_APP_H__
