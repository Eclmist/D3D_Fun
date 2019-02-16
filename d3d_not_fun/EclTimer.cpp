#include "EclTimer.h"

using namespace std::chrono;

EclTimer::EclTimer()
{
    last = steady_clock::now();
}

float EclTimer::Mark()
{
    const auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    return frameTime.count();
}

float EclTimer::Peek() const
{
    return duration<float>(steady_clock::now() - last).count();
}