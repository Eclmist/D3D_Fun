#include "EclTimer.h"

using namespace std::chrono;

EclTimer::EclTimer()
{
    m_last = steady_clock::now();
}

float EclTimer::mark()
{
    const auto old = m_last;
    m_last = steady_clock::now();
    const duration<float> frameTime = m_last - old;
    return frameTime.count();
}

float EclTimer::peek() const
{
    return duration<float>(steady_clock::now() - m_last).count();
}