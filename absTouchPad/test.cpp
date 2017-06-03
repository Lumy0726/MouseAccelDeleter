#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib") 
#pragma comment(lib, "D:\\Folder\\LinkProgramFiles\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\dinput8.lib")
#pragma comment(lib, "D:\\Folder\\LinkProgramFiles\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\dxguid.lib")
using namespace std;

//mecro

//type, class
class TimeU {
private:
	static bool flag;
	static __int64 ticFreq;
	__int64 startTic;

public:
	TimeU() { init(); resetTime(); }
	int getTime()const { return int((getTicStatic() - startTic) * 1000000 / ticFreq); }
	void resetTime() { startTic = getTicStatic(); }
	void cycleWait(int cycle) { int temp1 = 0, temp2 = 0; while ((temp2 = getTime() % cycle) >= temp1)temp1 = temp2; }

	static void init(){
		if (!flag) {
			LARGE_INTEGER temp;
			QueryPerformanceFrequency(&temp);
			ticFreq = temp.QuadPart;
			flag = true;
		}
	}
	static __int64 getTicStatic() {
		LARGE_INTEGER temp;
		QueryPerformanceCounter(&temp);
		return temp.QuadPart;
	}
};
bool TimeU::flag = false;
__int64 TimeU::ticFreq = 0;

//global variable
IDirectInput* m_pDI = NULL;												//DirectInput 객체.
IDirectInputDevice* m_pMouse = NULL;							//DirectInputDevice 객체.

//function
HRESULT InitDirectInput(HWND hWnd);
HRESULT GetMouseDelta(int * x, int * y);
void ExitMouse();

//main function
int main(int argn, char * argv[], char * env[]) {
	int x = 0, y = 0, dx =0, dy = 0;
	TimeU timeU;
	HRESULT hr;
	hr = InitDirectInput(GetConsoleWindow());
	if (FAILED(hr)) { cout << "ERRER!"; return 0; }
	cout << "Any key to Start\n";
	while (!_kbhit());
	timeU.resetTime();
	while (_kbhit()) { _getch(); }
	while (!_kbhit()) {
		timeU.cycleWait(50000);
		hr = GetMouseDelta(&dx, &dy);
		x += dx, y += dy;
		if (FAILED(hr)) { cout << "ERRER!"; return 0; }
		cout << setw(3) << x << "," << setw(3) << y << endl;
	}
	ExitMouse();
	return 0;
}

//function define
HRESULT InitDirectInput(HWND hWnd) {
	HRESULT hr;
	//Direct Input 객체 생성.
	//hr = DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	hr = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	if (FAILED(hr)) return hr;
	//DirectInputDevice 객체 생성.
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(hr))return hr;
	//마우스 정보에 대한 데이터 포맷 설정.
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;
	//윈도우의 모드 설정.
	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr)) return hr;
	return S_OK;
}
HRESULT GetMouseDelta(int * x, int * y) {
	if (NULL != m_pMouse) {
		DIMOUSESTATE dims;								//DirectInput의 마우스 상태 구조체.
		HRESULT hr;
		//장치 획득.
		hr = m_pMouse->Acquire();
		hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims);

		if (FAILED(hr)) return hr;
		*x = dims.lX; *y = dims.lY;
	}
	return S_OK;
}
void ExitMouse() {
	if (m_pMouse) {
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}
	if (m_pDI) {
		m_pDI->Release();
		m_pDI = NULL;
	}
}
//verge
