
// abdTP_directXDlg.h : 헤더 파일
//

#pragma once
#include <dinput.h>
#include "MMTimers.h"

// CabdTP_directXDlg 대화 상자
class CabdTP_directXDlg : public CDialogEx
{
// 생성입니다.
public:
	CabdTP_directXDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	IDirectInput* m_pDI;											//DirectInput 객체.
	IDirectInputDevice* m_pMouse;							//DirectInputDevice 객체.
	CMMTimers* m_Timer;												//멀티미디어 타이머.
	HRESULT InitDirectInput();
	HRESULT UpdateInputState();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABDTP_DIRECTX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMMTimer(WPARAM xParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	int x, y;
	CString m_MouseData;
	BOOL m_center;
	BOOL m_left;
	BOOL m_right;
	virtual BOOL DestroyWindow();
};
