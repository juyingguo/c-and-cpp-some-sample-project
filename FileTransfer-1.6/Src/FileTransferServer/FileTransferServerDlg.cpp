/*******************************版权信息*************************************
 * 文件名称 : FileTransferServerDlg.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : implementation file
 ****************************************************************************/

#include "stdafx.h"
#include "FileTransferServer.h"
#include "FileTransferServerDlg.h"
#include "ServerControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTransferServerDlg dialog

CFileTransferServerDlg::CFileTransferServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTransferServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTransferServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileTransferServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTransferServerDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileTransferServerDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTransferServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWZER, OnBrowzer)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_INTERRUPT, OnInterrupt)
	ON_BN_CLICKED(IDC_LOAD_HELP, OnLoadHelp)
	ON_BN_CLICKED(IDC_CLOSE_CONNECT, OnCloseConnect)
	ON_MESSAGE(WM_PROGRESS,OnProgress)					//进度更新消息映射
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)					//最小化到托盘消息映射
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTransferServerDlg message handlers

	
BOOL CFileTransferServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	Init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileTransferServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_MINIMIZE)			//最小化到托盘
	{
		_control->ToTray();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileTransferServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileTransferServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/*---------------------------------------------------------------------------
 * 函数名称 : Init
 * 函数功能 : 初始化视图层相关属性
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::Init()
{
	_model = new CServerModel(m_hWnd);
	_control = new CServerControl(IDR_MAINFRAME, this, _model);
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnTimer
 * 函数功能 : 定时器消息处理函数
 * 入口参数 : UINT nIDEvent, 定时器ID
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_UPDATE:
//		_control->UpdateSpeed(this);
		_control->UpdateControl(this);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
/*---------------------------------------------------------------------------
 * 函数名称 : WindowProc
 * 函数功能 : 提供视图层消息处理的接口
 * 入口参数 : UINT message, WPARAM wParam, LPARAM lParam
 * 出口参数 : 消息处理的结果
 ---------------------------------------------------------------------------*/
LRESULT CFileTransferServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (_control->JudgeMessage(message) == TRUE)
	{
		_control->MessageProcess(message);
	}
	if (message == WM_CLOSE)
	{
		if (_model->GetState() != INIT)
		{
			_model->CloseConnect();
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnProgress
 * 函数功能 : 进度更新消息
 * 入口参数 : WPARAM wParam, LPARAM lParam
 * 出口参数 : TRUE
 * 更新日期 : 2010.03.04
 * 更新原因 : 更新数据类型为Size，调整Size类型的转化方式
 ---------------------------------------------------------------------------*/
LRESULT CFileTransferServerDlg::OnProgress(WPARAM wParam, LPARAM lParam)
{
	int _progress = _control->GetProgress();
	////////////////////////2010.03.04更新///////////////////////////////////////
	//Size _offset = _model->GetOffset();
	//CString _strOffset = _control->SizeToString(_offset, RADIX);
	//GetDlgItem(IDC_OFFSET)->SetWindowText(_strOffset);
	/////////////////////////////////////////////////////////////////////////////

	/*****************************更新记录**************************************/
	//更新日期 : 2010.03.08
	//更新原因 : 更新视图对已传输的百分比显示
	CString _strOffset;
	_strOffset.Format("%d", _progress);
	_strOffset = _strOffset + _T(" %");
	GetDlgItem(IDC_OFFSET)->SetWindowText(_strOffset);
	m_ctlProgress.SetPos(_progress);
	/***************************************************************************/
	
	return TRUE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnShowTask
 * 函数功能 : 最小化到托盘的消息处理
 * 入口参数 : WPARAM wParam,LPARAM lParam
 * 出口参数 : TRUE
 ---------------------------------------------------------------------------*/
LRESULT CFileTransferServerDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
		return FALSE;
	if (lParam == WM_LBUTTONDBLCLK)
	{
		this->ShowWindow(SW_SHOW);
		_control->DeleteTray();
	}
	return TRUE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnBrowzer
 * 函数功能 : 委托控制对象完成文件路径的设置
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnBrowzer() 
{
	// TODO: Add your control notification handler code here
	_control->Browzer();
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnCloseConnect
 * 函数功能 : 委托模型关闭TCP连接
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnCloseConnect() 
{
	// TODO: Add your control notification handler code here
	_model->CloseConnect();
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnSend
 * 函数功能 : 委托控制对象完成传输文件的初始流程控制
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	_control->BeginTransfer();
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnInterrupt
 * 函数功能 : 委托模型完成中断传输的功能
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnInterrupt() 
{
	// TODO: Add your control notification handler code here
	_model->InterruptTransfer();
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnAbout
 * 函数功能 : 打开“关于”对话框
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg _dlg;
	_dlg.DoModal();
}
/*---------------------------------------------------------------------------
 * 函数名称 : OnLoadHelp
 * 函数功能 : 委托控制对象载入帮助信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnLoadHelp() 
{
	// TODO: Add your control notification handler code here
	_control->LoadHelp();
}
