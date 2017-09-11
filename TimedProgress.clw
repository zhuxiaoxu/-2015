; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTimedProgressDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "timedprogress.h"
LastPage=0

ClassCount=4
Class1=TProgressTimeToComplete
Class2=CTimedProgressApp
Class3=CAboutDlg
Class4=CTimedProgressDlg

ResourceCount=2
Resource1=IDD_TIMEDPROGRESS_DIALOG
Resource2=IDD_ABOUTBOX (English (U.S.))

[CLS:TProgressTimeToComplete]
Type=0
BaseClass=CProgressCtrl
HeaderFile=ProgressTimeToComplete.h
ImplementationFile=ProgressTimeToComplete.cpp
LastObject=TProgressTimeToComplete
Filter=W
VirtualFilter=NWC

[CLS:CTimedProgressApp]
Type=0
BaseClass=CWinApp
HeaderFile=TimedProgress.h
ImplementationFile=TimedProgress.cpp
LastObject=CTimedProgressApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=TimedProgressDlg.cpp
ImplementationFile=TimedProgressDlg.cpp
LastObject=CAboutDlg

[CLS:CTimedProgressDlg]
Type=0
BaseClass=CDialog
HeaderFile=TimedProgressDlg.h
ImplementationFile=TimedProgressDlg.cpp
LastObject=CTimedProgressDlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_TIMEDPROGRESS_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDC_PROGRESS3,msctls_progress32,1350565889
Control2=IDC_PROGRESS4,msctls_progress32,1350565888
Control3=IDC_PROGRESS1,msctls_progress32,1350565889

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

