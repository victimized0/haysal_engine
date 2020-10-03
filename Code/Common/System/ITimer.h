#ifndef INTERFACE_TIMER_H
#define INTERFACE_TIMER_H
#pragma once

struct ITimer
{
	virtual			~ITimer() {}

	virtual void	Start() = 0;
	virtual void	Stop() = 0;
	virtual void	Reset() = 0;
	virtual void	Tick() = 0;

	virtual bool	IsTimerPaused() const = 0;
	virtual float	GetTotalTime() const = 0;
	virtual float	GetFrameTime() const = 0;
	virtual float	TicksToSeconds(int64 ticks) const = 0;
	virtual float	GetFrameRate() = 0;
	virtual int64	GetTicksPerSecond() = 0;
};

#endif INTERFACE_TIMER_H
