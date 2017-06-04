#include <iostream>
#include <fstream>
#include <iomanip>
//#include <assert.h>
#include <windows.h>
#include <conio.h>
#include "Keyboard.h"
#include "Fraction.h"
#include "TimeU.h"
#include "DImouse.h"
using namespace std;

//mecro
#define MAX_X 1536
#define MAX_Y 863
#define MAX_V 1000
#define MOVE_D 100
//type, class

//variable
int sampleData[MAX_V];
int dataNum[MAX_V];
int timeBlock = 10;

//function
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

//main
int main(int arg_num, char * argv[], char * env[]) {
	enum State{LEFT, STOP, RIGHT};
	TimeU timeU;
	DImouse Mouse;
	int dx, dy, dxSum, time, playCounter = 0, counter = 0, zeroCounter = 0;
	int tempInt1;
	State state;
	ifstream fileI;
	ofstream fileO;

	system("color f0");
	system("title Mouse Accel Deletor Optimizer");
	cout << "Mouse Accel Deletor Optimizer" << endl;
	cout << "Any Key to start, Any key to end";
	fileI.open("Mouse_Accel_Optimizer.conf");
	if (fileI) {
		fileI >> timeBlock;
		while (true) {
			fileI >> tempInt1;
			fileI >> sampleData[tempInt1];
			fileI >> dataNum[tempInt1];
			if (fileI.eof())break;
		}
		fileI.close();
	}
	while (_kbhit()) { _getch(); }//���������.
	while (!_kbhit());
	while (_kbhit()) { _getch(); }//���������.
	state = STOP;
	dxSum = 0;
	Mouse.reset();
	timeU.resetTime();
	time = timeU.getTimeM();
	while (!_kbhit()) {
		if (timeU.getTimeM() - time < timeBlock) { gotoxy(0, 4); cout << "Fast enough"; }
		else { gotoxy(0, 4); cout << "           "; }
		timeU.cycleWait(TimeU::mSec2uSec(timeBlock));
		time = timeU.getTimeM();
		Mouse.GetDelta(&dx, &dy);
		if (dx > 0) {
			if (state != RIGHT) {
				if (dxSum > MOVE_D) {
					tempInt1 = int(dxSum / double(counter - zeroCounter) + 0.5);
					sampleData[tempInt1] = (sampleData[tempInt1] * dataNum[tempInt1] + dxSum) / (dataNum[tempInt1] + 1);
					dataNum[tempInt1]++;
					gotoxy(0, 0);
					cout << "--------right" << playCounter++ << endl;
					cout << setw(4) << tempInt1 << "  " << setw(5) << dxSum << "  " << setw(3) << endl;
				}
				state = RIGHT;
				dxSum = dx;
				counter = 1;
			}
			else {
				dxSum += dx;
				counter++;
			}
			zeroCounter = 0;
		}
		else if (dx) {
			if (state != LEFT) {
				if (dxSum > MOVE_D) {
					tempInt1 = int(dxSum / double(counter - zeroCounter) + 0.5);
					sampleData[tempInt1] = (sampleData[tempInt1] * dataNum[tempInt1] + dxSum) / (dataNum[tempInt1] + 1);
					dataNum[tempInt1]++;
					gotoxy(0, 0);
					cout << "left---------" << playCounter++ << endl;
					cout << setw(4) << tempInt1 << "  " << setw(5) << dxSum << "  " << setw(3) << endl;
				}
				state = LEFT;
				dxSum = -dx;
				counter = 1;
			}
			else {
				dxSum -= dx;
				counter++;
			}
			zeroCounter = 0;
		}
		else {
			counter++;
			zeroCounter++;
			if (zeroCounter > 10) {
				state = STOP;
			}
		}
	}
	fileO.open("Mouse_Accel_Optimizer.conf");
	fileO << timeBlock << endl;
	fileO << setw(4) << 0 << "  " << setw(5) << sampleData[0] << "  " << setw(3) << dataNum[0] << endl;
	fileO << setw(4) << 1 << "  " << setw(5) << sampleData[1] << "  " << setw(3) << dataNum[1] << endl;
	for (int loop1 = 2; loop1 < MAX_V; loop1++) {
		if (sampleData[loop1]) {
			fileO << setw(4) << loop1 << "  " << setw(5) << sampleData[loop1] << "  " << setw(3) << dataNum[loop1] << endl;
		}
	}
	fileO.close();
}
//function define

//verge


