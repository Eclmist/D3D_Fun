#pragma once
#include <chrono>

class EclTimer
{
public:
    EclTimer();
    float Mark();
    float Peek() const;

private:
    std::chrono::steady_clock::time_point last;
};

