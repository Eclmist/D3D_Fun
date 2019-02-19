#pragma once
#include <chrono>

class EclTimer
{
public:
    EclTimer() noexcept;
    float Mark() noexcept;
    float Peek() const noexcept;

private:
    std::chrono::steady_clock::time_point m_PreviousTime;
};

