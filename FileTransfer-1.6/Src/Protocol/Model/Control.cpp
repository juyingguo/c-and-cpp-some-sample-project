/*******************************版权信息*************************************
 * 文件名称 : Control.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : implementation of the CControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Control.h"
#include "Model.h"
#include "Tray.h"
#include "Format.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControl::CControl()
{
	
}

CControl::~CControl()
{

}

/***************************************************************************/
/*********************以下是CControl对外提供的接口**************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : JudgeMessage
 * 函数功能 : 判断消息类型是否为预定义消息
 * 入口参数 : UINT message, 消息代号
 * 出口参数 : 预定义的消息类型返回TRUE, 否则返回FALSE
 ---------------------------------------------------------------------------*/
int CControl::JudgeMessage(UINT message)
{
	for (UINT i = START_INDEX_MSG; i <= END_INDEX_MSG; i++)
	{
		if (message == i)
		{
			return TRUE;
		}
	}
	return FALSE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : LoadHelp
 * 函数功能 : 载入帮助文件信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::LoadHelp()
{
	DWORD len = MAX_FILE_PATH_LEN;
	char curdir[MAX_FILE_PATH_LEN];
	DWORD total;
	total = GetCurrentDirectory(len,curdir);
	strcat(curdir,HELP_PATH);
	ShellExecute(m_hWnd,"open",curdir,"","",SW_SHOW );
}
/*---------------------------------------------------------------------------
 * 函数名称 : GetProgress
 * 函数功能 : 获取文件传输的进度
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.01
 * 更新原因 : 调整偏移类型为DWORD
 ---------------------------------------------------------------------------*/
int CControl::GetProgress()
{
	FileInfo _fileInfo = _model->GetFileInfo();
	Size _offSet = _model->GetOffset();
	Size _block = _fileInfo._fileSize / BLOCK_SIZE;
	Size _transferred = _offSet / BLOCK_SIZE;
	
	int _progress = (int)((_transferred * (MAX_RANGE - MIN_RANGE)) / (_block * STEP));
	
	return _progress;
}
/*---------------------------------------------------------------------------
 * 函数名称 : UpdateControl
 * 函数功能 : 更新传输速度并显示
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.04
 * 更新原因 : 增加时间更新信息
 ---------------------------------------------------------------------------*/
void CControl::UpdateControl(CDialog *_pView)
{
	////////////////////////更新速度信息/////////////////////////////////////////
	_endOffset = _model->GetOffset();
	CString _strSpeed = GetSpeed();
	if (_strSpeed != _T(""))
	{
		_pView->GetDlgItem(_speedID)->SetWindowText(_strSpeed);		//更新
	}
	
	/////////////////////////////////////////////////////////////////////////////

	///////////////////////更新剩余时间信息//////////////////////////////////////
	CString _strRemainedTime = GetRemainedTime();
	if (_strRemainedTime != _T(""))
	{
		_pView->GetDlgItem(_remainedTimeID)->SetWindowText(_strRemainedTime);
	}
	/////////////////////////////////////////////////////////////////////////////

	//////////////////////更新完毕后重置起始标记/////////////////////////////////
	_startOffset = _endOffset;
}
/*---------------------------------------------------------------------------
 * 函数名称 : ToTray
 * 函数功能 : 最小化到托盘
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::ToTray()
{
	CTray _tray(m_hWnd, _uID);
	_tray.ToTray();
}
/*---------------------------------------------------------------------------
 * 函数名称 : DeleteTray
 * 函数功能 : 删除托盘
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::DeleteTray()
{
	CTray _tray(m_hWnd, _uID);
	_tray.DeleteTray();
}
/*---------------------------------------------------------------------------
 * 函数名称 : SizeToString
 * 函数功能 : 提供从Size到CString转化的接口
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
CString CControl::SizeToString(Size _size, int _radix)
{
//	CSizeConvert _convert(_size, _radix);
//	return _convert.ToString();
	return CFormat::ToString(_size, _radix);
}

/*********************以下方法延迟到子类中实现******************************/


void CControl::Browzer()
{
	
}

int CControl::IsValidPath()
{
	return TRUE;
}

void CControl::ConnectProcess(CString _strRemoteAddr)
{
	
}

void CControl::BeginTransfer()
{
	
}
/*---------------------------------------------------------------------------
 * 函数名称 : MessageProcess
 * 函数功能 : 视图消息的处理过程，这里提供一般的处理方法
 * 入口参数 : UINT message, 消息类型
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::MessageProcess(UINT message)
{
	switch (message)
	{
	case CONNECT_SUCCESS:			//连接成功
		ResetView();
		break;
	case TRANSFERRING_FILE:			//传输文件
		UpdateView();
		break;
	case TRANSFER_OVER:				//传输完毕
		::KillTimer(m_hWnd, TIMER_UPDATE);
		AfxMessageBox("传输完毕！");
		ResetView();
		break;
	case INTERRUPT_TRANSFER:		//中断传输
		::KillTimer(m_hWnd, TIMER_UPDATE);
		AfxMessageBox("传输中断！");
		ResetView();
		break;
	case CLOSE_CONNECT:
		::KillTimer(m_hWnd, TIMER_UPDATE);
		AfxMessageBox("连接已断开！");
		Init();
		break;
	}
}

/***************************************************************************/
/*********************以下是CControl的内部方法******************************/
/***************************************************************************/
/*---------------------------------------------------------------------------
 * 函数名称 : Init
 * 函数功能 : 初始化CControl的属性和视图信息
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::Init()
{
	InitControlID();
	InitStateMap();
	InitControlStateMap();
	InitFilePath();
	ResetView();
}
/*---------------------------------------------------------------------------
 * 函数名称 : InitStateMap
 * 函数功能 : 初始化状态字符信息映射表
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::InitStateMap()
{
	_strStateMap[INIT]			= _T("初始态");
	_strStateMap[CONNECTED]		= _T("已连接");
	_strStateMap[TRANSFERRING]	= _T("传输中");
}
/*---------------------------------------------------------------------------
 * 函数名称 : ResetControl
 * 函数功能 : 重置视图控件状态
 * 入口参数 : CDialog *_pView, 视图对象
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::ResetControl(CDialog *_pView)
{
	int _state = _model->GetState();
	_pView->GetDlgItem(_stateID)->SetWindowText(_strStateMap[_state]);
	_pView->GetDlgItem(_speedID)->SetWindowText(_T(""));
	_pView->GetDlgItem(_remainedTimeID)->SetWindowText(_T(""));
	for (int i = 0; i < _controlNum; i++)
	{
		_pView->GetDlgItem(_controlID[i])->EnableWindow(_ctlStateMap[i][_state]);
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : UpdateView
 * 函数功能 : 发送消息给视图，提示更新
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CControl::UpdateView()
{
	::SendMessage(m_hWnd, WM_PROGRESS, 0, 0);
}
/*---------------------------------------------------------------------------
 * 函数名称 : GetSpeed
 * 函数功能 : 获取实时传输速度信息
 * 入口参数 : 无
 * 出口参数 : 实时速度
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整数据类型为Size，更新Size转化为CString的方式
 ---------------------------------------------------------------------------*/
CString CControl::GetSpeed()
{
	Size _speed = _endOffset - _startOffset;
	long _ratio = ELAPSE_DEFAULT / ELAPSE_UPDATE;
	return CFormat::ToString(_speed, _ratio);
}
/*---------------------------------------------------------------------------
 * 函数名称 : GetRemainedTime
 * 函数功能 : 获取剩余传输时间
 * 入口参数 : 无
 * 出口参数 : 剩余时间的CString形式
 ---------------------------------------------------------------------------*/
CString CControl::GetRemainedTime()
{
	Size _remainedTime;

	Size _speed = _endOffset - _startOffset;

	Size _offset = _model->GetOffset();
	FileInfo _fileInfo = _model->GetFileInfo();
	Size _fileSize = _fileInfo._fileSize;

	Size _remainedSize = _fileSize - _offset;

	if (_speed <= 0)
	{
		return _T("");
	}
	else
	{
		_remainedTime = _remainedSize / _speed;
		return CFormat::ToString(_remainedTime);
	}
}

/********************以下方法延迟到子类实现*********************************/

void CControl::InitControlID()
{
	
}

void CControl::InitControlStateMap()
{
	
}

void CControl::InitFilePath()
{
	
}

void CControl::ResetView()
{

}


void CControl::SetFileInfo()
{

}

