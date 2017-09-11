// TimedProgressDlg.h : header file
//

#if !defined(AFX_TIMEDPROGRESSDLG_H__770907F7_A66F_11D4_85BA_00A0CC5F2805__INCLUDED_)
#define AFX_TIMEDPROGRESSDLG_H__770907F7_A66F_11D4_85BA_00A0CC5F2805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTimedProgressDlg dialog
#include "ProgressTimeToComplete.h"

struct threadInfo

{

	//UINT nMilliSecond;

	CProgressCtrl* pctrlProgress1;
	CProgressCtrl* pctrlProgress3;
	CProgressCtrl* pctrlProgress4;

};


#include "ProgressTimeToComplete.h"
class CTimedProgressDlg : public CDialog
{
// Construction
	//BOOL Process(CString famc);
public:
	CTimedProgressDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(CTimedProgressDlg)
	enum { IDD = IDD_TIMEDPROGRESS_DIALOG };
//	CSliderCtrl	m_slider;
//	CButton	m_btnGo;
//	CButton	m_btnStop;
	//CProgressCtrl	m_progress8;
	//CProgressCtrl	m_progress7;
	//CProgressCtrl	m_progress6;
	//CProgressCtrl	m_progress5;
	CProgressCtrl	m_progress4;
	CProgressCtrl	m_progress3;
	//TProgressTimeToComplete	m_progress2;
	CProgressCtrl	m_progress1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimedProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTimedProgressDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	HANDLE hThread;

	DWORD ThreadID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEDPROGRESSDLG_H__770907F7_A66F_11D4_85BA_00A0CC5F2805__INCLUDED_)
