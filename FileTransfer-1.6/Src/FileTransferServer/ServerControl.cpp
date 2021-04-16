/*******************************版权信息*************************************
 * 文件名称 : ServerControl.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : implementation of the CServerControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileTransferServer.h"
#include "ServerControl.h"
#include "FileTransferServerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 * 函数名称 : CServerControl
 * 函数功能 : 构造方法，初始化控制对象
 * 入口参数 : int _uID, CFileTransferServerDlg * _pView, CModel * _model
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
CServerControl::CServerControl(int _uID, CFileTransferServerDlg * _pView, CModel * _model)
{
	this->_pView	= _pView;
	this->_model	= _model;
	this->m_hWnd	= _pView->m_hWnd;
	this->_uID		= _uID;
	Init();
}

CServerControl::~CServerControl()
{

}

/***************************************************************************/
/*********************以下是CServerControl对外提供的接口********************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : Browzer
 * 函数功能 : 设置待传输的文件的路径
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整Size到CString类型的转化方式
 ---------------------------------------------------------------------------*/
void CServerControl::Browzer()
{
	CFileDialog * pDlg;
	pDlg = new CFileDialog(TRUE);
	if (pDlg->DoModal() == IDOK)
	{
		CString _strFilePath = pDlg->GetPathName();
		_pView->GetDlgItem(IDC_FILE_PATH)->SetWindowText(_strFilePath);

		SetFileInfo();
		FileInfo _fileInfo = _model->GetFileInfo();

		//char _strSize[MAX_BUF_LEN];
		//_ltoa(_fileInfo._size,_strSize,10);
		//CString _strSize = GetSize(_fileInfo._size, 10);
		//////////////////////2010.03.04/////////////////////////////////////////////
		//CString _strSize = SizeToString(_fileInfo._fileSize, RADIX);
		/////////////////////////////////////////////////////////////////////////////

		/*****************************更新记录**************************************/
		//更新日期 : 2010.03.08
		//更新原因 : 调整文件大小的显示方式为主流的显示格式
		CString _strSize = CFormat::FormatFileSize(_fileInfo._fileSize);
		/***************************************************************************/
		
		_pView->GetDlgItem(IDC_FILE_SIZE)->SetWindowText(_strSize);
	}
	delete pDlg;
	pDlg = NULL;
}
/*---------------------------------------------------------------------------
 * 函数名称 : IsValidPath
 * 函数功能 : 判断文件路径是否合法
 * 入口参数 : 无
 * 出口参数 : 合法返回TRUE, 否则返回FALSE
 ---------------------------------------------------------------------------*/
int CServerControl::IsValidPath()
{
	CString _strFilePath;
	_pView->GetDlgItem(IDC_FILE_PATH)->GetWindowText(_strFilePath);
	CFile file;
	if (file.Open(_strFilePath,CFile::modeRead))
	{
		return TRUE;
	}
	return FALSE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : BeginTransfer
 * 函数功能 : 文件传输的入口点流程控制
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerControl::BeginTransfer()
{
	if (IsValidPath() == TRUE)
	{
		if (_model->TransferFileInfo() == SOCKET_ERROR)
		{
			AfxMessageBox("传输文件失败！");
		}
	}
	else
	{
		AfxMessageBox("文件路径不合法！");
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : MessageProcess
 * 函数功能 : 视图层的消息处理
 * 入口参数 : UINT message, 视图层的消息
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerControl::MessageProcess(UINT message)
{
	switch (message)
	{
	case CONNECT_SUCCESS:
		_pView->GetDlgItem(IDC_REMOTE_ADDR)->SetWindowText(_model->GetRemoteAddr());
		ResetView();
		return;
	case RECV_FILE_INFO_ACK:
		ResetControl(_pView);
		::SetTimer(m_hWnd, TIMER_UPDATE, ELAPSE_UPDATE, NULL);		//开启速度更新定时器
		_startOffset = _model->GetOffset();
		return;
	}
	CControl::MessageProcess(message);
}

/***************************************************************************/
/*********************以下是CServerControl的内部方法************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : InitControlID
 * 函数功能 : 初始化视图控件的ID信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerControl::InitControlID()
{
	_stateID		= IDC_STATE;
	_speedID		= IDC_SPEED;
	_remainedTimeID	= IDC_REMAINED_TIME;

	_controlNum		= SERVER_CTL_NUM;
	_controlID[0]	= IDC_BROWZER;
	_controlID[1]	= IDC_SEND;
	_controlID[2]	= IDC_INTERRUPT;
	_controlID[3]	= IDC_CLOSE_CONNECT;
}

/*---------------------------------------------------------------------------
 * 函数名称 : InitControlStateMap
 * 函数功能 : 初始化控件状态映射表
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerControl::InitControlStateMap()
{
	int _temp[SERVER_CTL_NUM][STATE_NUM] = {
		{TRUE, TRUE, FALSE},
		{FALSE, TRUE, FALSE},
		{FALSE, FALSE, TRUE},
		{FALSE, TRUE, FALSE},
	};
	for (int i = 0; i < SERVER_CTL_NUM; i++)
	{
		for (int j = 0; j < STATE_NUM; j++)
		{
			_ctlStateMap[i][j] = _temp[i][j];
		}
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : ResetView
 * 函数功能 : 重置视图相关信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerControl::ResetView()
{
	ResetControl(_pView);

	CString _str = _model->GetRemoteAddr();
	_pView->GetDlgItem(IDC_OFFSET)->SetWindowText(_T(""));
	_pView->GetDlgItem(IDC_FILE_PATH)->SetWindowText(_T(""));
	_pView->GetDlgItem(IDC_FILE_SIZE)->SetWindowText(_T(""));
	//_pView->GetDlgItem(IDC_SPEED)->SetWindowText(_T(""));
	
	_pView->GetDlgItem(IDC_IP_ADDRESS)->SetWindowText(_model->GetAddress());
	_pView->GetDlgItem(IDC_REMOTE_ADDR)->SetWindowText(_model->GetRemoteAddr());

	_pView->m_ctlProgress.SetRange(MIN_RANGE,MAX_RANGE);
	_pView->m_ctlProgress.SetStep(STEP);
	_pView->m_ctlProgress.SetPos(MIN_RANGE);
}
/*---------------------------------------------------------------------------
 * 函数名称 : SetFileInfo
 * 函数功能 : 设置文件信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerControl::SetFileInfo()
{
	CString _strFilePath;
	_pView->GetDlgItem(IDC_FILE_PATH)->GetWindowText(_strFilePath);

	FileInfo _fileInfo;
	memset(&_fileInfo, 0, sizeof(FileInfo));

	/*****************************更新记录**************************************/
	//更新日期 : 2010.03.08
	//更新原因 : 调整GetSize接口
	//	CFileModel _fileModel(_strFilePath);
	_fileInfo._fileSize = CFileModel::GetSize(_strFilePath);
	/***************************************************************************/

	CFile file;
	if (file.Open(_strFilePath,CFile::modeRead))
	{
//		_fileInfo._size			= file.GetLength();
		CString _strFileName	= file.GetFileName();
		memcpy(_fileInfo._name, _strFileName, _strFileName.GetLength());
		_model->SetFilePath(_strFilePath);		//将信息保存到模型中
		_model->SetFileInfo(_fileInfo);
		file.Close();
	}
}

