#include "stdafx.h"
#include <windows.h>
#include "MMTimers.h"

CMMTimers::CMMTimers(UINT resolution) : timerRes(0), timerId(0){
	TIMECAPS tc;
	if (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(TIMECAPS))) {
		timerRes = min(max(tc.wPeriodMin, resolution), tc.wPeriodMax);
		timeBeginPeriod(timerRes);
	}
}

CMMTimers::~CMMTimers() {
	stopTimer();
	if (0 != timerRes) {
		timeEndPeriod(timerRes);
		timerRes = 0;
	}
}

bool CMMTimers::startTimer(UINT period, bool oneShot, LPTIMECALLBACK lpTimeProc) {
	bool res = false;
	MMRESULT result;
	result = timeSetEvent(period, timerRes, lpTimeProc, (DWORD)this, oneShot?TIME_ONESHOT:TIME_PERIODIC);
	if (NULL != result) {
		timerId = (UINT)result;
		res = true;
	}
	return res;
}

bool CMMTimers::stopTimer() {
	MMRESULT result;
	result = timeKillEvent(timerId);
	if (TIMERR_NOERROR == result) {
		timerId = 0;
	}
	return TIMERR_NOERROR == result;
}