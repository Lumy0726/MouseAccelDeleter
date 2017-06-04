#pragma once
#include <windows.h>

class TimeU {
private:
	static bool flag;
	static __int64 tickFreq;
	__int64 startTick;

public:
	TimeU();
	__int64 getTime()const;
	int getTimeM()const;
	__int64 getTimeR();
	int getTimeRM();
	void resetTime();
	void cycleWait(__int64 cycle);

	//static function.
	static void init();
	static __int64 getTickStatic();
	static __int64 getTickFreq();
	static __int64 mSec2uSec(int input);
	static int uSec2mSec(__int64 input);
};
