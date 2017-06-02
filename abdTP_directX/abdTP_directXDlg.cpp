
// abdTP_directXDlg.cpp : ���� ����
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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CabdTP_directXDlg ��ȭ ����



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


// CabdTP_directXDlg �޽��� ó����

BOOL CabdTP_directXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	HRESULT hr;
	//�׻� NULL�� �ʱ�ȭ.
	m_pDI = NULL;
	m_pMouse = NULL;

	hr = InitDirectInput();
	if (FAILED(hr)) {
		return FALSE;
	}
	//��Ƽ�̵�� Ÿ�̸� ����.
	m_Timer = new CMMTimers(10);
	MyhWnd = GetSafeHwnd();
	m_Timer->startTimer(50, FALSE, TimerProcK);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CabdTP_directXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CabdTP_directXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CabdTP_directXDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	//Direct Input ��ü ����.
	hr = DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	if (FAILED(hr)) return hr;
	//DirectInputDevice ��ü ����.
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(hr))return hr;
	//���콺 ������ ���� ������ ���� ����.
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;
	//�������� ��� ����.
	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) return hr;
	return S_OK;
}

HRESULT CabdTP_directXDlg::UpdateInputState() {
	if (NULL != m_pMouse) {
		DIMOUSESTATE dims;								//DirectInput�� ���콺 ���� ����ü.
		HRESULT hr;
		//��ġ ȹ��.
		hr = m_pMouse->Acquire();
		hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims);

		if (FAILED(hr)) return hr;
		//��ȭ������ ���� �ؽ�Ʈ ��Ʈ�ѿ� ���콺 �������� ��ġ ���.
		m_MouseData.Format((CString)"(%3d, %3d, %3d )", dims.lX, dims.lY, dims.lZ);
		//���콺 ��ư�� ������ ���� �Ǻ�.
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