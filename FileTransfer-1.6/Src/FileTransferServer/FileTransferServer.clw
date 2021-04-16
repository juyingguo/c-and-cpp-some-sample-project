; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFileTransferServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FileTransferServer.h"

ClassCount=4
Class1=CFileTransferServerApp
Class2=CFileTransferServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CProgressDlg
Resource3=IDD_FILETRANSFERSERVER_DIALOG

[CLS:CFileTransferServerApp]
Type=0
HeaderFile=FileTransferServer.h
ImplementationFile=FileTransferServer.cpp
Filter=N

[CLS:CFileTransferServerDlg]
Type=0
HeaderFile=FileTransferServerDlg.h
ImplementationFile=FileTransferServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CFileTransferServerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=FileTransferServerDlg.h
ImplementationFile=FileTransferServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_FILETRANSFERSERVER_DIALOG]
Type=1
Class=CFileTransferServerDlg
ControlCount=28
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FILE_PATH,static,1350696960
Control3=IDC_BROWZER,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_FILE_SIZE,static,1350696960
Control6=IDC_STATIC,static,1342308352
Control7=IDC_IP_ADDRESS,static,1350696960
Control8=IDC_SEND,button,1342242816
Control9=IDC_STATE,static,1350696960
Control10=IDC_ABOUT,button,1342242816
Control11=IDC_PROGRESS,msctls_progress32,1342177280
Control12=IDC_INTERRUPT,button,1342242816
Control13=IDC_OFFSET,static,1350696960
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_REMOTE_ADDR,static,1350696960
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,static,1342308352
Control21=IDC_LOAD_HELP,button,1342242816
Control22=IDC_STATIC,button,1342177287
Control23=IDC_CLOSE_CONNECT,button,1342242816
Control24=IDC_STATIC,static,1342308352
Control25=IDC_SPEED,static,1350696960
Control26=IDC_STATIC,static,1342308352
Control27=IDC_REMAINED_TIME,static,1350696960
Control28=IDC_STATIC,static,1342308352

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_PROGRESS
VirtualFilter=dWC

