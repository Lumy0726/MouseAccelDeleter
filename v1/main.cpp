#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
//#include <assert.h>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 
#include "Keyboard.h"
#include "Fraction.h"
using namespace std;

//mecro
#define OPTION_NUM 6
#define MAX_X 1536
#define MAX_Y 863
//type, class

//variable
C_fraction sensitive = C_fraction(28, 10);
C_fraction DR[OPTION_NUM];
int DD[OPTION_NUM] = { 4, 20, 40, 60, 80, 100 };
int timeBlock = 2;
bool info = false;

//function
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
int accelInv(int input) {
	for (int loop1 = OPTION_NUM - 1; loop1 >= 0; loop1--) {
		if (input < -DD[loop1] || DD[loop1] < input) {
			return (sensitive * C_fraction(input) * DR[loop1]).Get_value_int();
		}
	}
	return (sensitive * C_fraction(input)).Get_value_int();
}


//main
int main(int arg_num, char * argv[], char * env[]) {
	POINT cursor;
	int xPrev, yPrev, dx, dy, time, timePrev, setting = 0;
	bool flag = true;
	ifstream fileI;
	ofstream fileO;
	//double speedInv=0;

	//init code;
	timeBeginPeriod(1);
	DR[0] = C_fraction(60, 100);
	DR[1] = C_fraction(50, 100);
	DR[2] = C_fraction(45, 100);
	DR[3] = C_fraction(40, 100);
	DR[4] = C_fraction(35, 100);
	DR[5] = C_fraction(30, 100);
	C_keyboard::Check(C_keyboard::NUMPAD0);
	C_keyboard::Check(C_keyboard::NUMPAD1);
	C_keyboard::Check(C_keyboard::NUMPAD5);
	C_keyboard::Check(C_keyboard::ENTER);
	C_keyboard::Check(C_keyboard::SPACE);
	C_keyboard::Check(C_keyboard::UP);
	C_keyboard::Check(C_keyboard::DOWN);
	C_keyboard::Check(C_keyboard::Q);//sensitive;
	C_keyboard::Check(C_keyboard::A);
	C_keyboard::Check(C_keyboard::W);//Delete Ratio;
	C_keyboard::Check(C_keyboard::S);
	C_keyboard::Check(C_keyboard::E);//Delete Distance;
	C_keyboard::Check(C_keyboard::D);
	C_keyboard::Check(C_keyboard::R);//Time Block;
	C_keyboard::Check(C_keyboard::F);
	fileI.open("MouseAccelSave.conf");
	if (fileI) {
		int temp1, temp2;
		fileI >> temp1 >> temp2;
		sensitive = C_fraction(temp1, temp2);
		fileI >> temp1;
		if (OPTION_NUM == temp1) {
			for (int loop1 = 0; loop1 < OPTION_NUM; loop1++) {
				fileI >> DD[loop1];
				fileI >> temp1 >> temp2;
				DR[loop1] = C_fraction(temp1, temp2);
			}
			fileI >> timeBlock;
		}
		fileI.close();
	}
	system("title Mouse Accel Deletor!");
	system("color f0");
	cout << fixed << setprecision(2);
	cout << "ENTER:옵션조정/계속,  1번키패드:일시정지,  SPACE:종료,  5번키패드:설정가속조정,  위아래방향키:제거옵션번호수정,  0번키패드:info출력유무\n";
	cout << "감도:" << setw(5) << sensitive.Get_value() << "/Q,A/\n";
	cout << "옵션:" << (setting + 1) << ", info_option: " << boolalpha << info << "\n";
	for (int loop1 = 0; loop1 < OPTION_NUM; loop1++) {
		cout << "Delete_distance" << (loop1 + 1) << ':' << setw(4) << DD[loop1] << "/W,S/\n";
		cout << "Delete_Ratio" << (loop1 + 1) << ':' << setw(5) << DR[loop1].Get_value() << "/E,D/\n";
	}
	cout << "Time Block:" << setw(2) << timeBlock << "/R,F/\n";
	while (true) {
		{//mouse modify code.
			gotoxy(0, 0);
			cout << "작동중ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
			GetCursorPos(&cursor);
			timePrev = timeGetTime();
			xPrev = cursor.x; yPrev = cursor.y;
			while (_kbhit()) { _getch(); }
			while (!_kbhit()) {
				while ((int)timeGetTime() - timePrev < timeBlock);
				GetCursorPos(&cursor);
				time = timeGetTime();
				dx = cursor.x - xPrev;
				dy = cursor.y - yPrev;
				if (dx != 0 || dy != 0) {
					xPrev += accelInv(dx);
					yPrev += accelInv(dy);
					if (xPrev < 0)xPrev = 0;
					else if (xPrev > MAX_X) xPrev = MAX_X;
					if (yPrev < 0)yPrev = 0;
					else if (yPrev > MAX_Y) yPrev = MAX_Y;
					SetCursorPos(xPrev, yPrev);
					if (info) {
						cout << setw(3) << dx << ", " << setw(3) << dy << endl;
					}
				}
				timePrev = timeGetTime();
			}
		}//mouse modify code end.
		//user interface;
		gotoxy(0, 0);
		cout << "ENTER:옵션조정/계속,  1번키패드:일시정지,  SPACE:종료,  5번키패드:설정가속조정,  위아래방향키:제거옵션번호수정,  0번키패드:info출력유무\n";
		C_keyboard::Check(C_keyboard::ENTER);
		while (flag) {
			gotoxy(0, 1);
			cout << "감도:" << setw(5) << sensitive.Get_value() << "/Q,A/\n";
			cout << "옵션:" << (setting + 1) <<", info_option: " << boolalpha << info <<  "\n";
			for (int loop1 = 0; loop1 < OPTION_NUM; loop1++) {
				cout << "Delete_distance" << (loop1 + 1) << ':' << setw(4) << DD[loop1] << "/W,S/\n";
				cout << "Delete_Ratio" << (loop1 + 1) << ':' << setw(5) << DR[loop1].Get_value() << "/E,D/\n";
			}
			cout << "Time Block:" << setw(2) << timeBlock << "/R,F/\n";
			if (C_keyboard::Check_state(C_keyboard::NUMPAD1)) {
				while (C_keyboard::Check_state(C_keyboard::NUMPAD1));//1번때면.
				while (_kbhit()) { _getch(); }//버퍼지우고.
				while (!_kbhit());//버퍼다시들어올때가지 정지.
			}
			if (C_keyboard::Check_press(C_keyboard::ENTER)) { break; }
			if (C_keyboard::Check_press(C_keyboard::SPACE)) { flag = false; break; }
			if (C_keyboard::Check_press(C_keyboard::UP)) { setting--; }
			if (C_keyboard::Check_press(C_keyboard::DOWN)) { setting++; }
			if (C_keyboard::Check_press(C_keyboard::NUMPAD0)) { info = !info; }
			if (C_keyboard::Check_state(C_keyboard::NUMPAD5)) {
				if (C_keyboard::Check_state(C_keyboard::Q)) { sensitive = sensitive + C_fraction(1, 100); }
				if (C_keyboard::Check_state(C_keyboard::A)) { sensitive = sensitive - C_fraction(1, 100); }
				if (C_keyboard::Check_state(C_keyboard::W)) { DD[setting]++; }
				if (C_keyboard::Check_state(C_keyboard::S)) { DD[setting]--; }
				if (C_keyboard::Check_state(C_keyboard::E)) { DR[setting] = DR[setting] + C_fraction(1, 100); }
				if (C_keyboard::Check_state(C_keyboard::D)) { DR[setting] = DR[setting] - C_fraction(1, 100); }
				if (C_keyboard::Check_state(C_keyboard::R)) { timeBlock++; }
				if (C_keyboard::Check_state(C_keyboard::F)) { timeBlock--; }
			}
			else {
				if (C_keyboard::Check_press(C_keyboard::Q)) { sensitive = sensitive + C_fraction(1, 100); }
				if (C_keyboard::Check_press(C_keyboard::A)) { sensitive = sensitive - C_fraction(1, 100); }
				if (C_keyboard::Check_press(C_keyboard::W)) { DD[setting]++; }
				if (C_keyboard::Check_press(C_keyboard::S)) { DD[setting]--; }
				if (C_keyboard::Check_press(C_keyboard::E)) { DR[setting] = DR[setting] + C_fraction(1, 100); }
				if (C_keyboard::Check_press(C_keyboard::D)) { DR[setting] = DR[setting] - C_fraction(1, 100); }
				if (C_keyboard::Check_press(C_keyboard::R)) { timeBlock++; }
				if (C_keyboard::Check_press(C_keyboard::F)) { timeBlock--; }
			}
		}
		if (!flag)break;
	}
	fileO.open("MouseAccelSave.conf");
	/*C_fraction sensitive = C_fraction(27, 10);
	C_fraction DR[OPTION_NUM];
	int DD[OPTION_NUM] = { 4, 20, 40, 60, 80, 100 };
	int timeBlock = 2;*/
	if (fileO) {
		fileO << sensitive.Get_numer() << ' ' << sensitive.Get_denom() << "\n\n";
		fileO << OPTION_NUM << endl;
		for (int loop1 = 0; loop1 < OPTION_NUM; loop1++) {
			fileO << DD[loop1] << endl;
			fileO << DR[loop1].Get_numer() << ' ' << DR[loop1].Get_denom() << "\n\n";
		}
		fileO << timeBlock;
		fileO.close();
	}
	timeEndPeriod(1);
	return 0;
	//d2 = dx*dx + dy*dy;
	//speedInv = 1;
	//speedInv = -1;
	//speedInv = 1 / sqrt(d2);
	//speedInv = 6 / ((sqrt(d2) < 100) ? sqrt(d2) : 100);
	//speedInv = 1 / (0.5 + log(log(d2) + 1));
	//speedInv = 1 / (1.5 - 1 / double(d2));
	//speedInv = 1 / (1.1 - 1 / (double)d2);
	//speedInv = 1 / log10(d2 + 0.5);
	//speedInv = 10 / (double)d2;
	//speedInv = 0.0005 / log10(d2/10000.0 + 1);
	//speedInv = 3 * (temp = time - timePrev) / (d2 + 1);
	//speedInv = 3 * (temp = time - timePrev) / sqrt(d2);
	//xPrev += dx * speedInv; yPrev += dy * speedInv;
	//cout << setw(10) << sqrt(d2) << endl;
	//cout << "속도"<< setw(10) << 1 / speedInv << ", 거리" << setw(10) << sqrt(d2) << ", 거리/속도"<<setw(10) <<sqrt(d2)*speedInv<< endl;
	//cout << setw(4) << dx << ", " << setw(4) << dy << ", " << temp << ", " << speedInv << endl;
	//cursorPrev = cursor;
	//cout << setw(4) << cursor.x << ", " <<setw(4)<< cursor.y << ", " << setw(4)<<speed<<endl;
	//Sleep(5);
}
//function define

//verge











