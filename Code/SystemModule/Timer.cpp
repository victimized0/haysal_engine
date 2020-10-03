#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

bool Timer::Init()
{
    return true;
}

void Timer::Reset()
{
}

void Timer::Update()
{
}

float Timer::GetFrameTime() const
{
    return 0.0f;
}

void Timer::EnableTimer(bool enable)
{
}

bool Timer::IsTimerEnabled() const
{
    return false;
}

bool Timer::PauseTimer(bool pause)
{
    return false;
}

bool Timer::IsTimerPaused() const
{
    return false;
}

float Timer::TicksToSeconds(int64 ticks) const
{
    return 0.0f;
}

int64 Timer::GetTicksPerSecond()
{
    return int64();
}

float Timer::GetFrameRate()
{
    return 0.0f;
}
