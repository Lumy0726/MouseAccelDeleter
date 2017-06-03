#include "stdafx.h"
#include "MainProgram.h"
#include <iostream>

//variable

//function

//function define
int MainProgram(HWND hWnd) {
	int temp = 0;
	wchar_t str[STR_LENGTH];
	HDC hdc;
	while (temp <= 10) {
		hdc = GetDC(hWnd);
		wsprintf(str, L"%d", temp++);
		TextOut(hdc, 0, 0, L"asf", 0);
		Sleep(500);
		ReleaseDC(hWnd, hdc);
	}
	return 0;
}
//verge
