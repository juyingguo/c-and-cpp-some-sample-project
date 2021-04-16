; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFileTransferClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FileTransferClient.h"

ClassCount=4
Class1=CFileTransferClientApp
Class2=CFileTransferClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CProgressDlg
Resource3=IDD_FILETRANSFERCLIENT_DIALOG

[CLS:CFileTransferClientApp]
Type=0
HeaderFile=FileTransferClient.h
ImplementationFile=FileTransferClient.cpp
Filter=N

[CLS:CFileTransferClientDlg]
Type=0
HeaderFile=FileTransferClientDlg.h
ImplementationFile=FileTransferClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CFileTransferClientDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=FileTransferClientDlg.h
ImplementationFile=FileTransferClientDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_FILETRANSFERCLIENT_DIALOG]
Type=1
Class=CFileTransferClientDlg
ControlCount=28
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FILE_PATH,static,1350696960
Control3=IDC_BROWZER,button,1342242816
Control4=IDC_ABOUT,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CONNECT,button,1342242816
Control7=IDC_STATE,static,1350696960
Control8=IDC_PROGRESS,msctls_progress32,1342177280
Control9=IDC_INTERRUPT,button,1342242816
Control10=IDC_IPADDRESS,SysIPAddress32,1342242816
Control11=IDC_OFFSET,static,1350696960
Control12=IDC_STATIC,static,1342308352
Control13=IDC_FILE_SIZE,static,1350696960
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_LOAD_HELP,button,1342242816
Control21=IDC_CLOSE_CONNECT,button,1342242816
Control22=IDC_STATIC,static,1342308352
Control23=IDC_SPEED,static,1350696960
Control24=IDC_STATIC,static,1342308352
Control25=IDC_LOCAL_ADDR,static,1350696960
Control26=IDC_REMAINED_TIME,static,1350696960
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_PROGRESS
VirtualFilter=dWC

