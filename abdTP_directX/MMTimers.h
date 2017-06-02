#pragma once
#include <mmsystem.h>
#include <windows.h>

class CMMTimers {
public:
	CMMTimers(UINT resloution);
	virtual ~CMMTimers();
	UINT getTimerRes() { return timerRes; }
	bool startTimer(UINT period, bool oneShot, LPTIMECALLBACK lpTimeProc);
	bool stopTimer();

protected:
	UINT timerRes;
	UINT timerId;
};