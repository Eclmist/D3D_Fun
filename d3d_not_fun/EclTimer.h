#pragma once
#include <chrono>

class EclTimer
{
public:
    EclTimer();
    float mark();
    float peek() const;

private:
    std::chrono::steady_clock::time_point m_last;
};

