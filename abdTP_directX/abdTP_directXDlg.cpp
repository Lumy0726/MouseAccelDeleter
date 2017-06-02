
// abdTP_directXDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "abdTP_directX.h"
#include "abdTP_directXDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MMTIMER WM_USER + 2
HWND MyhWnd;
void CALLBACK TimerProcK(UINT id, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	PostMessage(MyhWnd, WM_MMTIMER, dw1, dw2);
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CabdTP_directXDlg 대화 상자



CabdTP_directXDlg::CabdTP_directXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABDTP_DIRECTX_DIALOG, pParent)
	, m_MouseData(_T(""))
	, m_center(FALSE)
	, m_left(FALSE)
	, m_right(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CabdTP_directXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MOUSE_STATUS, m_MouseData);
	DDX_Check(pDX, IDC_CENTER, m_center);
	DDX_Check(pDX, IDC_LEFT, m_left);
	DDX_Check(pDX, IDC_RIGHT, m_right);
}

BEGIN_MESSAGE_MAP(CabdTP_directXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MMTIMER, OnMMTimer)
END_MESSAGE_MAP()


// CabdTP_directXDlg 메시지 처리기

BOOL CabdTP_directXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	HRESULT hr;
	//항상 NULL로 초기화.
	m_pDI = NULL;
	m_pMouse = NULL;

	hr = InitDirectInput();
	if (FAILED(hr)) {
		return FALSE;
	}
	//멀티미디어 타이머 시작.
	m_Timer = new CMMTimers(10);
	MyhWnd = GetSafeHwnd();
	m_Timer->startTimer(50, FALSE, TimerProcK);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CabdTP_directXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CabdTP_directXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CabdTP_directXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CabdTP_directXDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (NULL != m_pMouse) {
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}
	if (NULL != m_pDI) {
		m_pDI->Release();
		m_pDI = NULL;
	}
	return CDialogEx::DestroyWindow();
}

HRESULT CabdTP_directXDlg::InitDirectInput() {
	HRESULT hr;
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	//Direct Input 객체 생성.
	hr = DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	if (FAILED(hr)) return hr;
	//DirectInputDevice 객체 생성.
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(hr))return hr;
	//마우스 정보에 대한 데이터 포맷 설정.
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;
	//윈도우의 모드 설정.
	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) return hr;
	return S_OK;
}

HRESULT CabdTP_directXDlg::UpdateInputState() {
	if (NULL != m_pMouse) {
		DIMOUSESTATE dims;								//DirectInput의 마우스 상태 구조체.
		HRESULT hr;
		//장치 획득.
		hr = m_pMouse->Acquire();
		hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims);

		if (FAILED(hr)) return hr;
		//대화상자의 정적 텍스트 건트롤에 마우스 포인터의 위치 출력.
		m_MouseData.Format((CString)"(%3d, %3d, %3d )", dims.lX, dims.lY, dims.lZ);
		//마우스 버튼의 눌려짐 여부 판별.
		if (dims.rgbButtons[0] & 0x80) m_left = TRUE;
		else m_left = FALSE;
		if (dims.rgbButtons[1] & 0x80) m_right = TRUE;
		else m_right = FALSE;
		if (dims.rgbButtons[2] & 0x80) m_center = TRUE;
		else m_center = FALSE;

		UpdateData(FALSE);
	}
	return S_OK;
}

afx_msg LRESULT CabdTP_directXDlg::OnMMTimer(WPARAM xParam, LPARAM lParam) {
	HRESULT hr;
	hr = UpdateInputState();
	if (FAILED(hr)) {
		m_Timer->stopTimer();
		MessageBox((CString)"Error Reading Input state", (CString)"DirectInput Sample", MB_ICONERROR | MB_OK);
	}
	return 0;
}