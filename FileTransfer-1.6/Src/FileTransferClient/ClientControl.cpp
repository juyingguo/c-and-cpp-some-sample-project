/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ClientControl.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * ����ԭ�� : ������Size�ṹ��������������
 * �ļ�˵�� : implementation of the CClientControl class.
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
 * �������� : CClientControl
 * �������� : ���캯������ʼ�����ƶ���
 * ��ڲ��� : int _uID, CFileTransferClientDlg * _pView, CModel * _model
 * ���ڲ��� : ��
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
/*******************������CClientControl�����ṩ�Ľӿ�**********************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : Browzer
 * �������� : �����ļ�·��
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientControl::Browzer()
{
	CString _strFilePath;
	char szName[MAX_FILE_PATH_LEN];
	BROWSEINFO _browzerInfo;
	ZeroMemory(&_browzerInfo,sizeof(BROWSEINFO));
	_browzerInfo.hwndOwner		= _pView->GetSafeHwnd();
	_browzerInfo.pszDisplayName	= szName;
	_browzerInfo.lpszTitle		= _T("��ѡ���ļ���");
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
* �������� : IsValidPath
* �������� : �ж��ļ�·���Ƿ�Ϸ�
* ��ڲ��� : ��
* ���ڲ��� : �Ϸ�����TRUE, ���򷵻�FALSE
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
 * �������� : ConnectProcess
 * �������� : ����TCP���ӵ����̿���
 * ��ڲ��� : CString _strRemoteAddr, �ؼ���server�˵�ַ
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientControl::ConnectProcess(CString _strRemoteAddr)
{
	if (inet_addr(_strRemoteAddr) == INADDR_NONE)
	{
		AfxMessageBox("��������ַ���ò��Ϸ���");
	}
	else
	{
		_model->SetRemoteAddr(_strRemoteAddr);
		_model->BeginConnectThread();
	}
}
/*---------------------------------------------------------------------------
 * �������� : BeginTransfer
 * �������� : ���俪�������̿���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientControl::BeginTransfer()
{
	if (IsValidPath() == TRUE)
	{
		SetFileInfo();		//����ģ�͵��ļ���Ϣ
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
 * �������� : MessageProcess
 * �������� : ��ͼ�����Ϣ����
 * ��ڲ��� : UINT message, ��ͼ�����Ϣ
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientControl::MessageProcess(UINT message)
{
	switch (message)
	{
	case SEND_FILE_INFO_REQ:
		BeginTransfer();
		if (_model->GetState() == TRANSFERRING)
		{
			::SetTimer(m_hWnd, TIMER_UPDATE, ELAPSE_UPDATE, NULL);		//�����ٶȸ��¶�ʱ��
			_startOffset = _model->GetOffset();
		}
		return;
	}
	CControl::MessageProcess(message);
}

/***************************************************************************/
/*******************������CClientControl���ڲ�����**************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : InitControlID
 * �������� : ��ʼ����ͼ�ؼ�ID��Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
* �������� : InitControlStateMap
* �������� : ��ʼ���ؼ�״̬ӳ���
* ��ڲ��� : ��
* ���ڲ��� : ��
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
 * �������� : InitFilePath
 * �������� : ��ʼ���ļ�·��ΪĬ��ֵ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
* �������� : ResetView
* �������� : ������ͼ�����Ϣ
* ��ڲ��� : ��
* ���ڲ��� : ��
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
 * �������� : SetFileInfo
 * �������� : ����Model���ļ�·��
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.04
 * ����ԭ�� : �����ļ���С�Ļ�ȡ��ʽ
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
	_model->SetOffset(_offset);			//�����ļ��ϵ�����λ��
	/////////////////////////////////////////////////////////////////////////////
}

/*---------------------------------------------------------------------------
 * �������� : SetFileSize
 * �������� : ������ͼ���ļ���С��Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.04
 * ����ԭ�� : ����Sizeת��ΪCString�ķ�ʽ
 * �������� : 2010.03.08
 * ����ԭ�� : ����Size����ʾ��ʽ
 ---------------------------------------------------------------------------*/
void CClientControl::SetFileSize()
{
	FileInfo _fileInfo = _model->GetFileInfo();
	/////////////////////////2010.03.04//////////////////////////////////////////
	//CString _strSize = SizeToString(_fileInfo._fileSize, RADIX);
	/////////////////////////////////////////////////////////////////////////////

	/*****************************���¼�¼**************************************/
	//�������� : 2010.03.08
	//����ԭ�� : ����Size����ʾ��ʽ
	CString _strSize = CFormat::FormatFileSize(_fileInfo._fileSize);
	/***************************************************************************/

	_pView->GetDlgItem(IDC_FILE_SIZE)->SetWindowText(_strSize);
}
