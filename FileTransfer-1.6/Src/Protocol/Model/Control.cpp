/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : Control.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : implementation of the CControl class.
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
/*********************������CControl�����ṩ�Ľӿ�**************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : JudgeMessage
 * �������� : �ж���Ϣ�����Ƿ�ΪԤ������Ϣ
 * ��ڲ��� : UINT message, ��Ϣ����
 * ���ڲ��� : Ԥ�������Ϣ���ͷ���TRUE, ���򷵻�FALSE
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
 * �������� : LoadHelp
 * �������� : ��������ļ���Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
 * �������� : GetProgress
 * �������� : ��ȡ�ļ�����Ľ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.01
 * ����ԭ�� : ����ƫ������ΪDWORD
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
 * �������� : UpdateControl
 * �������� : ���´����ٶȲ���ʾ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.04
 * ����ԭ�� : ����ʱ�������Ϣ
 ---------------------------------------------------------------------------*/
void CControl::UpdateControl(CDialog *_pView)
{
	////////////////////////�����ٶ���Ϣ/////////////////////////////////////////
	_endOffset = _model->GetOffset();
	CString _strSpeed = GetSpeed();
	if (_strSpeed != _T(""))
	{
		_pView->GetDlgItem(_speedID)->SetWindowText(_strSpeed);		//����
	}
	
	/////////////////////////////////////////////////////////////////////////////

	///////////////////////����ʣ��ʱ����Ϣ//////////////////////////////////////
	CString _strRemainedTime = GetRemainedTime();
	if (_strRemainedTime != _T(""))
	{
		_pView->GetDlgItem(_remainedTimeID)->SetWindowText(_strRemainedTime);
	}
	/////////////////////////////////////////////////////////////////////////////

	//////////////////////������Ϻ�������ʼ���/////////////////////////////////
	_startOffset = _endOffset;
}
/*---------------------------------------------------------------------------
 * �������� : ToTray
 * �������� : ��С��������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CControl::ToTray()
{
	CTray _tray(m_hWnd, _uID);
	_tray.ToTray();
}
/*---------------------------------------------------------------------------
 * �������� : DeleteTray
 * �������� : ɾ������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CControl::DeleteTray()
{
	CTray _tray(m_hWnd, _uID);
	_tray.DeleteTray();
}
/*---------------------------------------------------------------------------
 * �������� : SizeToString
 * �������� : �ṩ��Size��CStringת���Ľӿ�
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
CString CControl::SizeToString(Size _size, int _radix)
{
//	CSizeConvert _convert(_size, _radix);
//	return _convert.ToString();
	return CFormat::ToString(_size, _radix);
}

/*********************���·����ӳٵ�������ʵ��******************************/


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
 * �������� : MessageProcess
 * �������� : ��ͼ��Ϣ�Ĵ�����̣������ṩһ��Ĵ�����
 * ��ڲ��� : UINT message, ��Ϣ����
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CControl::MessageProcess(UINT message)
{
	switch (message)
	{
	case CONNECT_SUCCESS:			//���ӳɹ�
		ResetView();
		break;
	case TRANSFERRING_FILE:			//�����ļ�
		UpdateView();
		break;
	case TRANSFER_OVER:				//�������
		::KillTimer(m_hWnd, TIMER_UPDATE);
		AfxMessageBox("������ϣ�");
		ResetView();
		break;
	case INTERRUPT_TRANSFER:		//�жϴ���
		::KillTimer(m_hWnd, TIMER_UPDATE);
		AfxMessageBox("�����жϣ�");
		ResetView();
		break;
	case CLOSE_CONNECT:
		::KillTimer(m_hWnd, TIMER_UPDATE);
		AfxMessageBox("�����ѶϿ���");
		Init();
		break;
	}
}

/***************************************************************************/
/*********************������CControl���ڲ�����******************************/
/***************************************************************************/
/*---------------------------------------------------------------------------
 * �������� : Init
 * �������� : ��ʼ��CControl�����Ժ���ͼ��Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
 * �������� : InitStateMap
 * �������� : ��ʼ��״̬�ַ���Ϣӳ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CControl::InitStateMap()
{
	_strStateMap[INIT]			= _T("��ʼ̬");
	_strStateMap[CONNECTED]		= _T("������");
	_strStateMap[TRANSFERRING]	= _T("������");
}
/*---------------------------------------------------------------------------
 * �������� : ResetControl
 * �������� : ������ͼ�ؼ�״̬
 * ��ڲ��� : CDialog *_pView, ��ͼ����
 * ���ڲ��� : ��
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
 * �������� : UpdateView
 * �������� : ������Ϣ����ͼ����ʾ����
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CControl::UpdateView()
{
	::SendMessage(m_hWnd, WM_PROGRESS, 0, 0);
}
/*---------------------------------------------------------------------------
 * �������� : GetSpeed
 * �������� : ��ȡʵʱ�����ٶ���Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ʵʱ�ٶ�
 * �������� : 2010.03.04
 * ����ԭ�� : ������������ΪSize������Sizeת��ΪCString�ķ�ʽ
 ---------------------------------------------------------------------------*/
CString CControl::GetSpeed()
{
	Size _speed = _endOffset - _startOffset;
	long _ratio = ELAPSE_DEFAULT / ELAPSE_UPDATE;
	return CFormat::ToString(_speed, _ratio);
}
/*---------------------------------------------------------------------------
 * �������� : GetRemainedTime
 * �������� : ��ȡʣ�ഫ��ʱ��
 * ��ڲ��� : ��
 * ���ڲ��� : ʣ��ʱ���CString��ʽ
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

/********************���·����ӳٵ�����ʵ��*********************************/

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

