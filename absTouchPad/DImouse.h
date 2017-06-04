#pragma once
#include <dinput.h>

class DImouse {
protected:
	IDirectInput* m_pDI;											//DirectInput ��ü.
	IDirectInputDevice* m_pMouse;							//DirectInputDevice ��ü.
public:
	bool failed;

private:
	DImouse(const DImouse& input);
	DImouse& operator=(const DImouse& input);
public:
	DImouse();
	~DImouse();
	void reset();
	bool GetDelta(int * x, int * y);
	void ExitMouse();
};

//verge
