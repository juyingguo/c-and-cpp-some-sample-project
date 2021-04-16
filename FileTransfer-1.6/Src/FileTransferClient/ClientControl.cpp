/*******************************版权信息*************************************
 * 文件名称 : ClientControl.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整因Size结构变更而变更的内容
 * 文件说明 : implementation of the CClientControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileTransferClient.h"
#include "ClientControl.h"
#include "FileTransferClientDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 * 函数名称 : CClientControl
 * 函数功能 : 构造函数，初始化控制对象
 * 入口参数 : int _uID, CFileTransferClientDlg * _pView, CModel * _model
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
CClientControl::CClientControl(int _uID, CFileTransferClientDlg * _pView, CModel * _model)
{
	this->_pView	= _pView;
	this->_model	= _model;
	this->m_hWnd	= _pView->m_hWnd;
	this->_uID		= _uID;
	Init();
}

CClientControl::~CClientControl()
{

}

/***************************************************************************/
/*******************以下是CClientControl对外提供的接口**********************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : Browzer
 * 函数功能 : 设置文件路径
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::Browzer()
{
	CString _strFilePath;
	char szName[MAX_FILE_PATH_LEN];
	BROWSEINFO _browzerInfo;
	ZeroMemory(&_browzerInfo,sizeof(BROWSEINFO));
	_browzerInfo.hwndOwner		= _pView->GetSafeHwnd();
	_browzerInfo.pszDisplayName	= szName;
	_browzerInfo.lpszTitle		= _T("请选择文件夹");
	_browzerInfo.ulFlags		= BIF_RETURNFSANCESTORS;
	
	LPITEMIDLIST _itemIDList = SHBrowseForFolder(&_browzerInfo);
	if(_itemIDList == NULL)
	{
		_pView->GetDlgItem(IDC_FILE_PATH)->SetWindowText(_T(""));
		return;
	}
	CString _strTemp;
	SHGetPathFromIDList(_itemIDList, _strTemp.GetBuffer(MAX_FILE_PATH_LEN));
	_strTemp.ReleaseBuffer();
	_strFilePath = _strTemp;
	_pView->GetDlgItem(IDC_FILE_PATH)->SetWindowText(_strFilePath);
}
/*---------------------------------------------------------------------------
* 函数名称 : IsValidPath
* 函数功能 : 判断文件路径是否合法
* 入口参数 : 无
* 出口参数 : 合法返回TRUE, 否则返回FALSE
 ---------------------------------------------------------------------------*/
int CClientControl::IsValidPath()
{
	CFileFind _fileFind;
	CString _strFilePath;
	_pView->GetDlgItem(IDC_FILE_PATH)->GetWindowText(_strFilePath);
	if (_strFilePath.GetLength() == 0)
	{
		return FALSE;
	}
	CString _temp = _strFilePath;
	_temp = _temp + "*.*";
	if (_fileFind.FindFile(_temp))
	{
		return TRUE;
	}
	return FALSE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : ConnectProcess
 * 函数功能 : 建立TCP连接的流程控制
 * 入口参数 : CString _strRemoteAddr, 控件的server端地址
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::ConnectProcess(CString _strRemoteAddr)
{
	if (inet_addr(_strRemoteAddr) == INADDR_NONE)
	{
		AfxMessageBox("服务器地址设置不合法！");
	}
	else
	{
		_model->SetRemoteAddr(_strRemoteAddr);
		_model->BeginConnectThread();
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : BeginTransfer
 * 函数功能 : 传输开启的流程控制
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::BeginTransfer()
{
	if (IsValidPath() == TRUE)
	{
		SetFileInfo();		//设置模型的文件信息
		if (_model->TransferFileInfo() == TRUE)
		{
			ResetControl(_pView);
			SetFileSize();
		}
	}
	else
	{
		Browzer();
		if (IsValidPath() == TRUE)
		{
			SetFileInfo();
			if (_model->TransferFileInfo() == TRUE)
			{
				ResetControl(_pView);
				SetFileSize();
			}
		}
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : MessageProcess
 * 函数功能 : 视图层的消息处理
 * 入口参数 : UINT message, 视图层的消息
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::MessageProcess(UINT message)
{
	switch (message)
	{
	case SEND_FILE_INFO_REQ:
		BeginTransfer();
		if (_model->GetState() == TRANSFERRING)
		{
			::SetTimer(m_hWnd, TIMER_UPDATE, ELAPSE_UPDATE, NULL);		//开启速度更新定时器
			_startOffset = _model->GetOffset();
		}
		return;
	}
	CControl::MessageProcess(message);
}

/***************************************************************************/
/*******************以下是CClientControl的内部方法**************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : InitControlID
 * 函数功能 : 初始化视图控件ID信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::InitControlID()
{
	_stateID		= IDC_STATE;
	_speedID		= IDC_SPEED;
	_remainedTimeID	= IDC_REMAINED_TIME;

	_controlNum		= CLIENT_CTL_NUM;
	_controlID[0]	= IDC_BROWZER;
	_controlID[1]	= IDC_CONNECT;
	_controlID[2]	= IDC_INTERRUPT;
	_controlID[3]	= IDC_IPADDRESS;
	_controlID[4]	= IDC_CLOSE_CONNECT;
}

/*---------------------------------------------------------------------------
* 函数名称 : InitControlStateMap
* 函数功能 : 初始化控件状态映射表
* 入口参数 : 无
* 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::InitControlStateMap()
{
	int _temp[CLIENT_CTL_NUM][STATE_NUM] = {
		{TRUE, TRUE, FALSE},
		{TRUE, FALSE, FALSE},
		{FALSE, FALSE, TRUE},
		{TRUE, FALSE, FALSE},
		{FALSE, TRUE, FALSE},
	};
	for (int i = 0; i < CLIENT_CTL_NUM; i++)
	{
		for (int j = 0; j < STATE_NUM; j++)
		{
			_ctlStateMap[i][j] = _temp[i][j];
		}
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : InitFilePath
 * 函数功能 : 初始化文件路径为默认值
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::InitFilePath()
{
	char _szFilePath[MAX_FILE_PATH_LEN];
	memset(_szFilePath, 0, MAX_FILE_PATH_LEN);
	::GetCurrentDirectory(MAX_FILE_PATH_LEN, _szFilePath);
	strcat(_szFilePath, DEFAULT_FILE_PATH);
	::CreateDirectory(_szFilePath, NULL);
	_pView->GetDlgItem(IDC_FILE_PATH)->SetWindowText(_szFilePath);
}
/*---------------------------------------------------------------------------
* 函数名称 : ResetView
* 函数功能 : 重置视图相关信息
* 入口参数 : 无
* 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientControl::ResetView()
{
	ResetControl(_pView);

	_pView->GetDlgItem(IDC_OFFSET)->SetWindowText(_T(""));
	_pView->GetDlgItem(IDC_FILE_SIZE)->SetWindowText(_T(""));
	_pView->GetDlgItem(IDC_LOCAL_ADDR)->SetWindowText(_model->GetAddress());
	_pView->m_ctlAddress.SetWindowText(_model->GetRemoteAddr());

	_pView->m_ctlProgress.SetRange(MIN_RANGE,MAX_RANGE);
	_pView->m_ctlProgress.SetStep(STEP);
	_pView->m_ctlProgress.SetPos(MIN_RANGE);
}
/*---------------------------------------------------------------------------
 * 函数名称 : SetFileInfo
 * 函数功能 : 设置Model的文件路径
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整文件大小的获取方式
 ---------------------------------------------------------------------------*/
void CClientControl::SetFileInfo()
{
	CString _strFilePath;
	_pView->GetDlgItem(IDC_FILE_PATH)->GetWindowText(_strFilePath);

	_strFilePath = _strFilePath + _T("\\");
	FileInfo _fileInfo = _model->GetFileInfo();
	_strFilePath = _strFilePath + _fileInfo._name;
	_model->SetFilePath(_strFilePath);

	////////////////////////////2010.03.04///////////////////////////////////////
	Size _offset = CFileModel::GetSize(_strFilePath);
	if (_offset == INVALID_VALUE)
	{
		_offset = 0;
	}
	_model->SetOffset(_offset);			//设置文件断点续传位置
	/////////////////////////////////////////////////////////////////////////////
}

/*---------------------------------------------------------------------------
 * 函数名称 : SetFileSize
 * 函数功能 : 设置视图的文件大小信息
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整Size转化为CString的方式
 * 更新日期 : 2010.03.08
 * 更新原因 : 调整Size的显示方式
 ---------------------------------------------------------------------------*/
void CClientControl::SetFileSize()
{
	FileInfo _fileInfo = _model->GetFileInfo();
	/////////////////////////2010.03.04//////////////////////////////////////////
	//CString _strSize = SizeToString(_fileInfo._fileSize, RADIX);
	/////////////////////////////////////////////////////////////////////////////

	/*****************************更新记录**************************************/
	//更新日期 : 2010.03.08
	//更新原因 : 调整Size的显示方式
	CString _strSize = CFormat::FormatFileSize(_fileInfo._fileSize);
	/***************************************************************************/

	_pView->GetDlgItem(IDC_FILE_SIZE)->SetWindowText(_strSize);
}
