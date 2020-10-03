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
	void		Reset() override;
	void		Update() override;

	void		EnableTimer(bool enable) override;
	bool		IsTimerEnabled() const override;
	bool		PauseTimer(bool pause) override;
	bool		IsTimerPaused() const override;

	float		TicksToSeconds(int64 ticks) const override;
	int64		GetTicksPerSecond() override;
	float		GetFrameTime() const override;
	float		GetFrameRate() override;

private:


};

#endif //TIMER_H
