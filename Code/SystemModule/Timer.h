#ifndef TIMER_H
#define TIMER_H
#pragma once

#include <System\ITimer.h>

class Timer : public ITimer
{
public:
				Timer();
	virtual		~Timer();

	bool		Init();
	bool		IsTimerPaused() const override;

	void		Start() override;
	void		Stop() override;
	void		Reset() override;
	void		Tick() override;

	float		TicksToSeconds(int64 ticks) const override;
	int64		GetTicksPerSecond() override;
	float		GetFrameTime() const override;
	float		GetFrameRate() override;
	float		GetTotalTime() const;

private:
	bool		m_isPaused;
	double		m_frameTime;
	double		m_secsPerTick;
	int64       m_ticksPerSec;
	int64		m_prevTime;
	int64		m_currTime;
	int64		m_startTime;
	int64		m_pauseTime;
	int64		m_stopTime;
	uint32		m_frameCounter;
};

#endif //TIMER_H
