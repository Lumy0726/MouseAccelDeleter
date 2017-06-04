#include <iostream>
#include <fstream>
#include <iomanip>
//#include <cmath>
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

//type, class

//global variable
C_fraction sensitive = C_fraction(400, 100);
double DR[MAX_V];
int lastV = 0;
int timeBlock = 2;
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
double accelInv(int input, int timeU) {
	double dr;
	int V = absINT(input) * 1000 * timeBlock;
	int ratio = V % timeU;//timeBlock�ð�(ms)��ŭ �� �Ÿ�(�ӵ�)�� �Ҽ���*timeU.
	V /= timeU;//timeBlock�ð�(ms)��ŭ �� �Ÿ�(�ӵ�)�� ������.
	V = (V > lastV - 1) ? (lastV - 1) * tempnum1 : V * tempnum1;//���ǿ� ���� �ð��� �ִ��� �����ϰ� �ϱ� ���� ����.
	dr = (DR[V] * (timeU - ratio) + DR[V + 1] * (ratio)) / timeU;
	return sensitive.Get_value_int() * input * dr;
}


//main
int main(int arg_num, char * argv[], char * env[]) {
	DImouse Mouse;
	TimeU timeX, timeY;
	POINT cursor;
	double xPrev, yPrev;
	int dx, dy, time, unstableCounter;
	int tempInt1, tempInt2, tempInt3;
	bool flag, ctrlF = true;
	ifstream fileI;
	ofstream fileO;
	//double speedInv=0;

	//init code;
	C_keyboard::Check(C_keyboard::NUMPAD0);
	C_keyboard::Check(C_keyboard::NUMPAD1);
	C_keyboard::Check(C_keyboard::NUMPAD2);
	C_keyboard::Check(C_keyboard::NUMPAD5);
	C_keyboard::Check(C_keyboard::ENTER);
	C_keyboard::Check(C_keyboard::SPACE);
	C_keyboard::Check(C_keyboard::Q);//sensitive;
	C_keyboard::Check(C_keyboard::A);

	for (int loop1 = 2; loop1 < MAX_V; loop1++) {
		DR[loop1] = -1.0;
	}
	fileI.open("Mouse_Accel_Optimizer.conf");
	if (fileI) {
		int initV;
		fileI >> timeBlock;
		fileI >> tempInt1 >> tempInt2 >> tempInt1;//�ӵ� 0 �̵��� ����.
		fileI >> tempInt1 >> initV >> tempInt1;//initV==�ӵ� 1�� �̵���.
		DR[0] = initV / double(tempInt2);
		DR[1] = 1.0;
		if (fileI.eof()) {
			fileI.close();
			printf_s("�����Ͱ� �����ϴ�. Optimizer�� ������ �ּ���.");
			while (_kbhit()) { _getch(); }//���������.
			while (!_kbhit());//���۴ٽõ��ö����� ����.
			return 0;
		}
		while (true) {
			fileI >> tempInt1 >> tempInt2 >> tempInt3;
			if (tempInt3 >= 50) {
				DR[tempInt1] = initV / double(tempInt2);
			}
			lastV = tempInt1;
			if (fileI.eof())break;
		}
		fileI.close();
		for (int loop1 = 2; loop1 < lastV; loop1++) {
			if (DR[loop1] < -0.5) {
				DR[loop1] = DR[loop1 - 1];
			}
		}
	}
	else {
		printf_s("�����Ͱ� �����ϴ�. Optimizer�� ������ �ּ���.");
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
	cout << setw(50) << ' ' << endl;
	printf_s("ENTER:�ɼ�����/���,  0��:info�������,  1��:�Ͻ�����,  2��:ctrlŰ ���콺���� Ȱ��ȭ����,  SPACE:����\n");
	printf_s("QA:����,  5��:������������\n");
	printf_s("Time Block:%2d\n", timeBlock);
	printf_s("����:%6.3f/Q,A/\n", sensitive.Get_value());
	printf_s("�̵�����ǥ��:");
	if (info)printf_s(" true\n");
	else printf_s("false\n");
	printf_s("CTRLŰ�� ���콺�߾��̵�:");
	if (ctrlF)printf_s(" true\n");
	else printf_s("false\n");
	while (true) {
		{//mouse modify code.
			gotoxy(0, 0);
			printf_s("�۵��ߤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�");
			unstableCounter = 0;
			flag = false;
			while (_kbhit()) { _getch(); }
			GetCursorPos(&cursor);
			xPrev = (double)cursor.x; yPrev = (double)cursor.y;
			Mouse.reset();
			timeX.resetTime();
			timeY.resetTime();
			while (!_kbhit()) {
				//mouse modify main code.
				timeX.cycleWait(__int64(10));
				if (ctrlF && C_keyboard::Check_state(C_keyboard::CONTROL)) {
					xPrev = MAX_X / 2.0; timeX.resetTime();
					yPrev = MAX_Y / 2.0; timeY.resetTime();
					flag = true;
				}
				else if (Mouse.GetDelta(&dx, &dy)) {
					if (dx) xPrev += accelInv(dx, (int)timeX.getTimeR()); flag = true;
					if (dy) yPrev += accelInv(dy, (int)timeY.getTimeR()); flag = true;
				}
				else {
					printf_s("Mouse error!!!\n");
					while (!_kbhit());
					break;
				}

				if (flag) {
					SetCursorPos((int)xPrev, (int)yPrev);
					flag = false;
				}
				if ((int)xPrev < 0)xPrev = 0.0;
				else if ((int)xPrev > MAX_X) xPrev = (double)MAX_X;
				if ((int)yPrev < 0)yPrev = 0.0;
				else if ((int)yPrev > MAX_Y) yPrev = (double)MAX_Y;
				if (info) {
					if (dx != 0 || dy != 0) {
						cout << setw(4) << dx << ", " << setw(4) << dy << endl;
					}
				}
			}
		}//mouse modify code end.
		 //user interface;
		gotoxy(0, 0);
		cout << setw(50) << ' ' << endl;
		printf_s("ENTER:�ɼ�����/���,  0��:info�������,  1��:�Ͻ�����,  2��:ctrlŰ ���콺���� Ȱ��ȭ����,  SPACE:����\n");
		printf_s("QA:����,  5��:������������\n");
		printf_s("Time Block:%2d\n", timeBlock);
		C_keyboard::Check(C_keyboard::ENTER);
		flag = true;
		while (flag) {
			gotoxy(0, 4);
			printf_s("����:%6.3f/Q,A/\n", sensitive.Get_value());
			printf_s("�̵�����ǥ��:");
			if (info)printf_s(" true\n");
			else printf_s("false\n");
			printf_s("CTRLŰ�� ���콺�߾��̵�:");
			if (ctrlF)printf_s(" true\n");
			else printf_s("false\n");
			if (C_keyboard::Check_state(C_keyboard::NUMPAD1)) {
				while (C_keyboard::Check_state(C_keyboard::NUMPAD1));//1������.
				while (_kbhit()) { _getch(); }//���������.
				while (!_kbhit());//���۴ٽõ��ö����� ����.
			}
			if (C_keyboard::Check_press(C_keyboard::ENTER)) { break; }
			if (C_keyboard::Check_press(C_keyboard::SPACE)) { flag = false; break; }
			if (C_keyboard::Check_press(C_keyboard::NUMPAD0)) { info = !info; }
			if (C_keyboard::Check_press(C_keyboard::NUMPAD2)) { ctrlF = !ctrlF; }
			if (C_keyboard::Check_state(C_keyboard::NUMPAD5)) {
				time = timeX.getTimeM();
				while (timeX.getTimeM() - time < 5);
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
		fileO << sensitive.Get_numer() << ' ' << sensitive.Get_denom() << endl;
		fileO.close();
	}
	return 0;
}
//function define

//verge
