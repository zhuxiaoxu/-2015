// TimedProgress.h : main header file for the TIMEDPROGRESS application
//

#if !defined(AFX_TIMEDPROGRESS_H__770907F5_A66F_11D4_85BA_00A0CC5F2805__INCLUDED_)
#define AFX_TIMEDPROGRESS_H__770907F5_A66F_11D4_85BA_00A0CC5F2805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTimedProgressApp:
// See TimedProgress.cpp for the implementation of this class
//

class CTimedProgressApp : public CWinApp
{
public:
	CTimedProgressApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimedProgressApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTimedProgressApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEDPROGRESS_H__770907F5_A66F_11D4_85BA_00A0CC5F2805__INCLUDED_)
