#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 
using namespace std;

//mecro

//type, class
class TimeU {
private:
	static bool flag;
	static __int64 timeFreq;
	__int64 startTime;

public:
	TimeU() { init(); resetTime(); }
	int getTime()const { return int((getTimeStatic() - startTime) * 1000000 / timeFreq); }
	void resetTime() { startTime = getTimeStatic(); }

	static void init(){
		if (!flag) {
			LARGE_INTEGER temp;
			QueryPerformanceFrequency(&temp);
			timeFreq = temp.QuadPart;
			flag = true;
		}
	}
	static __int64 getTimeStatic() {
		LARGE_INTEGER temp;
		QueryPerformanceCounter(&temp);
		return temp.QuadPart;
	}
};

//global variable
bool TimeU::flag = false;
__int64 TimeU::timeFreq = 0;

//function

//main function
int main(int argn, char * argv[], char * env[]) {
	int time;
	TimeU timeU;

	timeBeginPeriod(1);
	while (_kbhit()) { _getch(); }
	while (!_kbhit());
	time = 0;
	while (_kbhit()) { _getch(); }
	while (!_kbhit()) {
		while (timeU.getTime() < 1000);
		timeU.resetTime();
		cout << time/1000 << endl;
		time++;
	}
	cout << time;
	timeEndPeriod(1);
	return 0;
}

//function define

//verge
