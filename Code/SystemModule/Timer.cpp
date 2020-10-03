#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
    : m_isPaused(false)
    , m_frameTime(-1.0)
    , m_secsPerTick(0.0)
    , m_ticksPerSec(0)
    , m_frameCounter(0)
    , m_prevTime(0)
    , m_currTime(0)
    , m_startTime(0)
    , m_pauseTime(0)
    , m_stopTime(0)
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&m_ticksPerSec);
    m_secsPerTick = 1.0 / static_cast<double>(m_ticksPerSec);

    Reset();
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
    QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);

    m_prevTime  = m_currTime;
    m_frameTime = 0.0f;
    m_startTime = m_currTime;
    m_stopTime  = 0;
	m_pauseTime = 0;
    m_isPaused  = false;
}

void Timer::Tick()
{
    if (m_isPaused)
    {
        m_frameTime = 0.0;
        return;
    }

    QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);
    m_frameTime = (m_currTime - m_prevTime) * m_secsPerTick;
    m_prevTime = m_currTime;

    if (m_frameTime < 0.0)
    {
        m_frameTime = 0.0;
    }
}

float Timer::GetFrameTime() const
{
    return m_isPaused ? 0.0f : m_frameTime;
}

bool Timer::IsTimerPaused() const
{
    return m_isPaused;
}

float Timer::TicksToSeconds(int64 ticks) const
{
    return float((double)ticks * m_secsPerTick);
}

int64 Timer::GetTicksPerSecond()
{
    return m_ticksPerSec;
}

float Timer::GetFrameRate()
{
    return m_frameTime;
}

float Timer::GetTotalTime() const
{
    if (m_isPaused)
    {
        return static_cast<float>(((m_stopTime - m_pauseTime) - m_startTime) * m_secsPerTick);
    }
    else
    {
        return static_cast<float>(((m_currTime - m_pauseTime) - m_startTime) * m_secsPerTick);
    }
}

void Timer::Start()
{
    if (m_isPaused)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);

        m_pauseTime += (m_stopTime - m_startTime);
        m_prevTime = m_startTime;

        m_stopTime = 0;
        m_isPaused = false;
    }
}

void Timer::Stop()
{
    if (!m_isPaused)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);

        m_stopTime = m_currTime;
        m_isPaused = true;
    }
}
