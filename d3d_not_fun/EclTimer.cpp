#include "ecltimer.h"

using namespace std::chrono;

EclTimer::EclTimer() noexcept
{
    m_PreviousTime = steady_clock::now();
}

float EclTimer::Mark() noexcept
{
    const auto old = m_PreviousTime;
    m_PreviousTime = steady_clock::now();
    const duration<float> frameTime = m_PreviousTime - old;
    return frameTime.count();
}

float EclTimer::Peek() const noexcept
{
    return duration<float>(steady_clock::now() - m_PreviousTime).count();
}