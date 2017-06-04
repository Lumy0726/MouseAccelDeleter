#include "TimeU.h"

TimeU::TimeU() { init(); resetTime(); }
__int64 TimeU::getTime()const { return (getTickStatic() - startTick) * __int64(1000000) / tickFreq; }
int TimeU::getTimeM()const{ return int((getTickStatic() - startTick) * __int64(1000) / tickFreq); }
__int64 TimeU::getTimeR(){
	__int64 temp = getTickStatic();
	__int64 returnValue = (temp - startTick) * __int64(1000000) / tickFreq;
	startTick = temp;
	return returnValue;
}
int TimeU::getTimeRM(){
	__int64 temp = getTickStatic();
	int returnValue = int((temp - startTick) * __int64(1000) / tickFreq);
	startTick = temp;
	return returnValue;
}
void TimeU::resetTime() { startTick = getTickStatic(); }
void TimeU::cycleWait(__int64 cycle) {
	__int64 temp1 = 0, temp2 = 0;
	if (getTimeM() > 1800 * 1000/*30Ка*/) resetTime();
	while ((temp2 = getTime() % cycle) >= temp1) {
		temp1 = temp2;
	}
}

//static function.
void TimeU::init() {
	if (!flag) {
		LARGE_INTEGER temp;
		QueryPerformanceFrequency(&temp);
		tickFreq = temp.QuadPart;
		flag = true;
	}
}
__int64 TimeU::getTickStatic() {
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return temp.QuadPart;
}
__int64 TimeU::getTickFreq() {
	init(); return tickFreq;
}
__int64 TimeU::mSec2uSec(int input) { return __int64(input) * __int64(1000); }
int TimeU::uSec2mSec(__int64 input) { return int(input / __int64(1000)); }
bool TimeU::flag = false;
__int64 TimeU::tickFreq = 0;