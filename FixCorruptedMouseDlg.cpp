
// FixCorruptedMouseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FixCorruptedMouse.h"
#include "FixCorruptedMouseDlg.h"
#include "afxdialogex.h"

#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int SET_MOUSE_TRAILS_EVENT_ID = 12345;

// Goes to HKEY_CURRENT_USER\Control Panel\Mouse::MouseTrails
// 0 and 1 mean disabled. 2 is the minimum.
// Mouse trails fix the mouse cursor corruption ATI video card users encounter.
const int MOUSE_TRAILS_LEVEL_2 = 2;
const int MOUSE_TRAILS_LEVEL_DISABLED = 0;

const int TRAY_ID = 1;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFixCorruptedMouseDlg dialog



CFixCorruptedMouseDlg::CFixCorruptedMouseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FIXCORRUPTEDMOUSE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFixCorruptedMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFixCorruptedMouseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFixCorruptedMouseDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CFixCorruptedMouseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EXIT, &CFixCorruptedMouseDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_BUTTON2, &CFixCorruptedMouseDlg::OnBnClickedAboutButton)
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE, OnTrayNotify)
END_MESSAGE_MAP()


// CFixCorruptedMouseDlg message handlers

BOOL CFixCorruptedMouseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(SET_MOUSE_TRAILS_EVENT_ID, 1000 /*milliseconds*/, NULL);
	::SystemParametersInfo(SPI_SETMOUSETRAILS, MOUSE_TRAILS_LEVEL_2, NULL, 0);


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	ShowWindow(SW_MINIMIZE);
	m_nidIconData.hIcon = m_hIcon;
	m_nidIconData.uFlags = NIF_MESSAGE;
	m_nidIconData.uFlags |= NIF_ICON;
	m_nidIconData.cbSize = sizeof(NOTIFYICONDATA);

	m_nidIconData.hWnd = m_hWnd;
	m_nidIconData.uID = TRAY_ID;

	m_nidIconData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;

	LPCWSTR lpszToolTip = L"Fixing your corrupted mouse cursor!";
	wcscpy_s(m_nidIconData.szTip, lpszToolTip);
	m_nidIconData.uFlags |= NIF_TIP;

	Shell_NotifyIcon(NIM_ADD, &m_nidIconData); 
	
	// Prevent multiple instances of the same app from running.
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "FixCorruptedMouseCursorApp");
	DWORD dwMutexWaitResult = WaitForSingleObject(hMutex, 0);
	if (dwMutexWaitResult != WAIT_OBJECT_0)
	{
		CloseHandle(hMutex);
		this->OnBnClickedExit();
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFixCorruptedMouseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_MINIMIZE)
	{
		this->bMinimized = true;
		::SystemParametersInfo(SPI_SETMOUSETRAILS, 10, NULL, 0);

		CDialog::OnSysCommand(nID, lParam);
	}
	else if (nID == SC_RESTORE)
	{
		this->bMinimized = false;
		CDialog::OnSysCommand(nID, lParam);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFixCorruptedMouseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFixCorruptedMouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFixCorruptedMouseDlg::OnBnClickedOk()
{
	// When disabling mouse trails, if the cursor was already corrupted it will revert to beeing corrupted.
	// If users quit this app, they should see the corrupted mouse cursor!!! Cause that's what they want!!!
	::SystemParametersInfo(SPI_SETMOUSETRAILS, MOUSE_TRAILS_LEVEL_DISABLED, NULL, 0);

	// TODO: Add your control notification handler code here
	// CDialog::OnOK();
	ShowWindow(SW_MINIMIZE);
	this->bMinimized = true;
}


void CFixCorruptedMouseDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == SET_MOUSE_TRAILS_EVENT_ID)
	{
		// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724947(v=vs.85).aspx
		// SPI_SETMOUSETRAILS 0x005D
		// 2 means activate mouse trails, shortest trail possible.

		::SystemParametersInfo(SPI_SETMOUSETRAILS, MOUSE_TRAILS_LEVEL_2, NULL, 0);

		if(this->bMinimized && !this->bHidden)
		{
			this->bHidden = true;
			ShowWindow(SW_HIDE);
		}
		
		if(!this->bMinimized && this->bHidden)
		{
			this->bHidden = false;
			ShowWindow(SW_RESTORE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CFixCorruptedMouseDlg::OnBnClickedCancel()
{
	ShowWindow(SW_MINIMIZE);
}


void CFixCorruptedMouseDlg::OnBnClickedExit()
{
	// When disabling mouse trails, if the cursor was already corrupted it will revert to beeing corrupted.
	// If users quit this app, they should see the corrupted mouse cursor!!! Cause that's what they want!!!

	// 0 means deactivate mouse trails.
	::SystemParametersInfo(SPI_SETMOUSETRAILS, MOUSE_TRAILS_LEVEL_DISABLED, NULL, 0);


	if(m_nidIconData.hWnd && m_nidIconData.uID>0)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nidIconData);
	}


	CDialog::OnCancel();
}


void CFixCorruptedMouseDlg::OnBnClickedAboutButton()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


LRESULT CFixCorruptedMouseDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uID;
	UINT uMsg;

	uID = (UINT)wParam;
	uMsg = (UINT)lParam;

	if (uID != TRAY_ID)
		return 0;

	switch (uMsg)
	{
		if (this->bMinimized && !this->bHidden)
		{
			this->bHidden = true;
			ShowWindow(SW_HIDE);
		}

		if (!this->bMinimized && this->bHidden)
		{
			this->bHidden = false;
			ShowWindow(SW_RESTORE);
		}


		case WM_MOUSEMOVE:
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
		case WM_RBUTTONDBLCLK:
			if(this->bMinimized)
			{
				this->bHidden = false;
				this->bMinimized = false;
				ShowWindow(SW_RESTORE);
			}
			break;
	}

	return 1;
}
