#include "DImouse.h"
#include <dinput.h>
#pragma comment(lib, "D:\\Folder\\LinkProgramFiles\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\dinput8.lib")
#pragma comment(lib, "D:\\Folder\\LinkProgramFiles\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\dxguid.lib")

DImouse::DImouse() :m_pDI(NULL), m_pMouse(NULL) { reset(); }
DImouse::~DImouse() { ExitMouse(); }
void DImouse::reset() {
	HRESULT hr;
	HINSTANCE hInstance = GetModuleHandle(0);
	HWND hWnd = GetConsoleWindow();
	//�ʱ�ȭ.
	failed = false;
	ExitMouse();
	//Direct Input ��ü ����.
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	if (FAILED(hr)) { failed = true; return; }
	//DirectInputDevice ��ü ����.
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(hr)) { failed = true; return; }
	//���콺 ������ ���� ������ ���� ����.
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) { failed = true; return; }
	//�������� ��� ����.
	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr)) { failed = true; return; }
}
bool DImouse::GetDelta(int * x, int * y) {
	if (m_pMouse == NULL) { failed = true; return false; }
	DIMOUSESTATE dims;								//DirectInput�� ���콺 ���� ����ü.
	HRESULT hr;
	//��ġ ȹ��.
	hr = m_pMouse->Acquire();
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims);

	if (FAILED(hr)) { failed = true; return false; }
	*x = dims.lX; *y = dims.lY;
	return true;
}
void DImouse::ExitMouse() {
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
