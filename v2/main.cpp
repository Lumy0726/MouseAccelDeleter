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
#define MAX_X 1536
#define MAX_Y 863
#define MAX_V 1000
//type, class

//variable
C_fraction sensitive = C_fraction(28, 10);
C_fraction DR[MAX_V];
int lastV = 0;
int timeBlock = 4;
bool info = false;

//function
volatile static int tempnum0 = 0;
volatile static int tempnum1 = 1;
volatile static int tempnum_1 = -1;
volatile static int tempnum2 = 2;
volatile static int tempnum_2 = -2;
int absINT(int input) {
	if (input > 0) {
		return input * tempnum2 / 2;//absINT�� ���� �ð��� �ִ��� �����ϰ� �ϱ� ���� ����.
	}
	else {
		return input * tempnum_2 / 2;
	}
}
int signINT(int input) {
	if (input > 0) {
		return tempnum1;
	}
	else {
		return tempnum_1;
	}
}
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
int accelInv(int input) {
	int temp = absINT(input) /*+ ((input<0) ? tempnum1 : tempnum0)*/;
	temp = (temp > lastV) ? lastV * tempnum1 : temp * tempnum1;//���ǿ� ���� �ð��� �ִ��� �����ϰ� �ϱ� ���� ����.
	return (sensitive * C_fraction(input /*2 * input - 1*/, 2) * DR[temp]).Get_value_int();
}


//main
int main(int arg_num, char * argv[], char * env[]) {
	POINT cursor;
	int xPrev, yPrev, dx, dy, time;
	int tempInt1, tempInt2, tempInt3;
	bool flag;
	ifstream fileI;
	ofstream fileO;
	//double speedInv=0;

	//init code;
	timeBeginPeriod(1);
	C_keyboard::Check(C_keyboard::NUMPAD0);
	C_keyboard::Check(C_keyboard::NUMPAD1);
	C_keyboard::Check(C_keyboard::NUMPAD5);
	C_keyboard::Check(C_keyboard::ENTER);
	C_keyboard::Check(C_keyboard::SPACE);
	C_keyboard::Check(C_keyboard::Q);//sensitive;
	C_keyboard::Check(C_keyboard::A);
	fileI.open("Mouse_Accel_Optimizer.conf");
	if (fileI) {
		int initV;
		fileI >> timeBlock;
		fileI >> tempInt1 >> tempInt1 >> tempInt1;//�ӵ� 0 ����.
		fileI >> tempInt1 >> initV >> tempInt1;//initV==�ӵ� 1�� �̵���.
		DR[1] = C_fraction(1);
		if (fileI.eof()) {
			fileI.close();
			cout << "�����Ͱ� �����ϴ�. Optimizer�� ������ �ּ���.";
			while (_kbhit()) { _getch(); }//���������.
			while (!_kbhit());//���۴ٽõ��ö����� ����.
			return 0;
		}
		while (true) {
			fileI >> tempInt1 >> tempInt2 >> tempInt3;
			if (tempInt3 >= 10) {
				DR[tempInt1] = C_fraction(initV, tempInt2);
			}
			lastV = tempInt1;
			if (fileI.eof())break;
		}
		fileI.close();
		for (int loop1 = 2; loop1 < lastV; loop1++) {
			if (DR[loop1] == C_fraction(0)) {
				DR[loop1] = DR[loop1 - 1];
			}
		}
	}
	else {
		cout << "�����Ͱ� �����ϴ�. Optimizer�� ������ �ּ���.";
		while (_kbhit()) { _getch(); }//���������.
		while (!_kbhit());//���۴ٽõ��ö����� ����.
		return 0;
	}
	fileI.open("Mouse_Accel_Deletor.conf");
	if (fileI) {
		fileI >> tempInt1 >> tempInt2;
		sensitive = C_fraction(tempInt1, tempInt2);
		fileI.close();
	}

	//need code.
	system("title Mouse Accel Deletor!");
	system("color f0");
	cout << fixed << setprecision(2);
	cout << "ENTER:�ɼ�����/���,  1��Ű�е�:�Ͻ�����,  SPACE:����,  5��Ű�е�:������������,  0��Ű�е�:info�������\n";
	cout << "Time Block:" << setw(2) << timeBlock << "/R,F/\n" << endl;
	cout << "����:" << setw(5) << sensitive.Get_value() << "/Q,A/\n";
	cout << "�̵�����ǥ��: " << boolalpha << info << endl;
	while (true) {
		{//mouse modify code.
			gotoxy(0, 0);
			cout << "�۵��ߤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n";
			GetCursorPos(&cursor);
			time = timeGetTime();
			xPrev = cursor.x; yPrev = cursor.y;
			flag = false;
			while (_kbhit()) { _getch(); }
			while (!_kbhit()) {
				while ((int)timeGetTime() - time < timeBlock);
				GetCursorPos(&cursor);
				dx = cursor.x - xPrev;
				dy = cursor.y - yPrev;
				if (dx) { xPrev += accelInv(dx); flag = true; }
				if (dy) { yPrev += accelInv(dy); flag = true; }
				if (flag) SetCursorPos(xPrev, yPrev);
				flag = false;
				if (xPrev < 0)xPrev = 0;
				else if (xPrev > MAX_X) xPrev = MAX_X;
				if (yPrev < 0)yPrev = 0;
				else if (yPrev > MAX_Y) yPrev = MAX_Y;
				if (info) {
					if (dx != 0 || dy != 0) {
						cout << setw(4) << dx << ", " << setw(4) << dy << endl;
					}
				}
				time = timeGetTime();
			}
		}//mouse modify code end.
		//user interface;
		gotoxy(0, 0);
		cout << "ENTER:�ɼ�����/���,  1��Ű�е�:�Ͻ�����,  SPACE:����,  5��Ű�е�:������������,  0��Ű�е�:info�������\n";
		cout << "Time Block:" << setw(2) << timeBlock << endl;
		C_keyboard::Check(C_keyboard::ENTER);
		flag = true;
		while (flag) {
			gotoxy(0, 2);
			cout << "����:" << setw(5) << sensitive.Get_value() << "/Q,A/\n";
			cout << "�̵�����ǥ��: " << boolalpha << info << endl;
			if (C_keyboard::Check_state(C_keyboard::NUMPAD1)) {
				while (C_keyboard::Check_state(C_keyboard::NUMPAD1));//1������.
				while (_kbhit()) { _getch(); }//���������.
				while (!_kbhit());//���۴ٽõ��ö����� ����.
			}
			if (C_keyboard::Check_press(C_keyboard::ENTER)) { break; }
			if (C_keyboard::Check_press(C_keyboard::SPACE)) { flag = false; break; }
			if (C_keyboard::Check_press(C_keyboard::NUMPAD0)) { info = !info; }
			if (C_keyboard::Check_state(C_keyboard::NUMPAD5)) {
				if (C_keyboard::Check_state(C_keyboard::Q)) { sensitive = sensitive + C_fraction(1, 100); }
				if (C_keyboard::Check_state(C_keyboard::A)) { sensitive = sensitive - C_fraction(1, 100); }
			}
			else {
				if (C_keyboard::Check_press(C_keyboard::Q)) { sensitive = sensitive + C_fraction(1, 100); }
				if (C_keyboard::Check_press(C_keyboard::A)) { sensitive = sensitive - C_fraction(1, 100); }
			}
		}
		if (!flag)break;
	}
	fileO.open("Mouse_Accel_Deletor.conf");
	if (fileO) {
		fileO << sensitive.Get_numer() << ' ' << sensitive.Get_denom();
		fileO.close();
	}
	timeEndPeriod(1);
	return 0;
}
//function define

//verge











