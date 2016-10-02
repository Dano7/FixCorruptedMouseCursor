
// FixCorruptedMouseDlg.h : header file
//

#pragma once


#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)

// CFixCorruptedMouseDlg dialog
class CFixCorruptedMouseDlg : public CDialog
{
// Construction
public:
	CFixCorruptedMouseDlg(CWnd* pParent = NULL);	// standard constructor

	NOTIFYICONDATA m_nidIconData;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIXCORRUPTEDMOUSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	private:
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	bool bMinimized = true;
	bool bHidden = false;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedAboutButton();
};
