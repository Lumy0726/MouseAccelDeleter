
// abdTP_directXDlg.h : ��� ����
//

#pragma once
#include <dinput.h>
#include "MMTimers.h"

// CabdTP_directXDlg ��ȭ ����
class CabdTP_directXDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CabdTP_directXDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	IDirectInput* m_pDI;											//DirectInput ��ü.
	IDirectInputDevice* m_pMouse;							//DirectInputDevice ��ü.
	CMMTimers* m_Timer;												//��Ƽ�̵�� Ÿ�̸�.
	HRESULT InitDirectInput();
	HRESULT UpdateInputState();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABDTP_DIRECTX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMMTimer(WPARAM xParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CString m_MouseData;
	BOOL m_center;
	BOOL m_left;
	BOOL m_right;
	virtual BOOL DestroyWindow();
};
