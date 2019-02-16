#pragma once
#include <chrono>

class EclTimer
{
public:
    EclTimer();
    float mark() noexcept;
    float peek() const noexcept;

private:
    std::chrono::steady_clock::time_point m_last;
};

