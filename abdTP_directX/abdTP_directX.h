
// abdTP_directX.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CabdTP_directXApp:
// �� Ŭ������ ������ ���ؼ��� abdTP_directX.cpp�� �����Ͻʽÿ�.
//

class CabdTP_directXApp : public CWinApp
{
public:
	CabdTP_directXApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CabdTP_directXApp theApp;