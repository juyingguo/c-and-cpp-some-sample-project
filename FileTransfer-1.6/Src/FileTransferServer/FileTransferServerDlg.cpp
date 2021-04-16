/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileTransferServerDlg.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : implementation file
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
	ON_MESSAGE(WM_PROGRESS,OnProgress)					//���ȸ�����Ϣӳ��
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)					//��С����������Ϣӳ��
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
	else if(nID == SC_MINIMIZE)			//��С��������
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
 * �������� : Init
 * �������� : ��ʼ����ͼ���������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::Init()
{
	_model = new CServerModel(m_hWnd);
	_control = new CServerControl(IDR_MAINFRAME, this, _model);
}
/*---------------------------------------------------------------------------
 * �������� : OnTimer
 * �������� : ��ʱ����Ϣ������
 * ��ڲ��� : UINT nIDEvent, ��ʱ��ID
 * ���ڲ��� : ��
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
 * �������� : WindowProc
 * �������� : �ṩ��ͼ����Ϣ����Ľӿ�
 * ��ڲ��� : UINT message, WPARAM wParam, LPARAM lParam
 * ���ڲ��� : ��Ϣ����Ľ��
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
 * �������� : OnProgress
 * �������� : ���ȸ�����Ϣ
 * ��ڲ��� : WPARAM wParam, LPARAM lParam
 * ���ڲ��� : TRUE
 * �������� : 2010.03.04
 * ����ԭ�� : ������������ΪSize������Size���͵�ת����ʽ
 ---------------------------------------------------------------------------*/
LRESULT CFileTransferServerDlg::OnProgress(WPARAM wParam, LPARAM lParam)
{
	int _progress = _control->GetProgress();
	////////////////////////2010.03.04����///////////////////////////////////////
	//Size _offset = _model->GetOffset();
	//CString _strOffset = _control->SizeToString(_offset, RADIX);
	//GetDlgItem(IDC_OFFSET)->SetWindowText(_strOffset);
	/////////////////////////////////////////////////////////////////////////////

	/*****************************���¼�¼**************************************/
	//�������� : 2010.03.08
	//����ԭ�� : ������ͼ���Ѵ���İٷֱ���ʾ
	CString _strOffset;
	_strOffset.Format("%d", _progress);
	_strOffset = _strOffset + _T(" %");
	GetDlgItem(IDC_OFFSET)->SetWindowText(_strOffset);
	m_ctlProgress.SetPos(_progress);
	/***************************************************************************/
	
	return TRUE;
}
/*---------------------------------------------------------------------------
 * �������� : OnShowTask
 * �������� : ��С�������̵���Ϣ����
 * ��ڲ��� : WPARAM wParam,LPARAM lParam
 * ���ڲ��� : TRUE
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
 * �������� : OnBrowzer
 * �������� : ί�п��ƶ�������ļ�·��������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnBrowzer() 
{
	// TODO: Add your control notification handler code here
	_control->Browzer();
}
/*---------------------------------------------------------------------------
 * �������� : OnCloseConnect
 * �������� : ί��ģ�͹ر�TCP����
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnCloseConnect() 
{
	// TODO: Add your control notification handler code here
	_model->CloseConnect();
}
/*---------------------------------------------------------------------------
 * �������� : OnSend
 * �������� : ί�п��ƶ�����ɴ����ļ��ĳ�ʼ���̿���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	_control->BeginTransfer();
}
/*---------------------------------------------------------------------------
 * �������� : OnInterrupt
 * �������� : ί��ģ������жϴ���Ĺ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnInterrupt() 
{
	// TODO: Add your control notification handler code here
	_model->InterruptTransfer();
}
/*---------------------------------------------------------------------------
 * �������� : OnAbout
 * �������� : �򿪡����ڡ��Ի���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg _dlg;
	_dlg.DoModal();
}
/*---------------------------------------------------------------------------
 * �������� : OnLoadHelp
 * �������� : ί�п��ƶ������������Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileTransferServerDlg::OnLoadHelp() 
{
	// TODO: Add your control notification handler code here
	_control->LoadHelp();
}
