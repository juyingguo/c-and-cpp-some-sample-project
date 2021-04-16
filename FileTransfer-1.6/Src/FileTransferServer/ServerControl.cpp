/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ServerControl.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : implementation of the CServerControl class.
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
 * �������� : CServerControl
 * �������� : ���췽������ʼ�����ƶ���
 * ��ڲ��� : int _uID, CFileTransferServerDlg * _pView, CModel * _model
 * ���ڲ��� : ��
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
/*********************������CServerControl�����ṩ�Ľӿ�********************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : Browzer
 * �������� : ���ô�������ļ���·��
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.04
 * ����ԭ�� : ����Size��CString���͵�ת����ʽ
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

		/*****************************���¼�¼**************************************/
		//�������� : 2010.03.08
		//����ԭ�� : �����ļ���С����ʾ��ʽΪ��������ʾ��ʽ
		CString _strSize = CFormat::FormatFileSize(_fileInfo._fileSize);
		/***************************************************************************/
		
		_pView->GetDlgItem(IDC_FILE_SIZE)->SetWindowText(_strSize);
	}
	delete pDlg;
	pDlg = NULL;
}
/*---------------------------------------------------------------------------
 * �������� : IsValidPath
 * �������� : �ж��ļ�·���Ƿ�Ϸ�
 * ��ڲ��� : ��
 * ���ڲ��� : �Ϸ�����TRUE, ���򷵻�FALSE
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
 * �������� : BeginTransfer
 * �������� : �ļ��������ڵ����̿���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerControl::BeginTransfer()
{
	if (IsValidPath() == TRUE)
	{
		if (_model->TransferFileInfo() == SOCKET_ERROR)
		{
			AfxMessageBox("�����ļ�ʧ�ܣ�");
		}
	}
	else
	{
		AfxMessageBox("�ļ�·�����Ϸ���");
	}
}
/*---------------------------------------------------------------------------
 * �������� : MessageProcess
 * �������� : ��ͼ�����Ϣ����
 * ��ڲ��� : UINT message, ��ͼ�����Ϣ
 * ���ڲ��� : ��
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
		::SetTimer(m_hWnd, TIMER_UPDATE, ELAPSE_UPDATE, NULL);		//�����ٶȸ��¶�ʱ��
		_startOffset = _model->GetOffset();
		return;
	}
	CControl::MessageProcess(message);
}

/***************************************************************************/
/*********************������CServerControl���ڲ�����************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : InitControlID
 * �������� : ��ʼ����ͼ�ؼ���ID��Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
 * �������� : InitControlStateMap
 * �������� : ��ʼ���ؼ�״̬ӳ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
 * �������� : ResetView
 * �������� : ������ͼ�����Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
 * �������� : SetFileInfo
 * �������� : �����ļ���Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerControl::SetFileInfo()
{
	CString _strFilePath;
	_pView->GetDlgItem(IDC_FILE_PATH)->GetWindowText(_strFilePath);

	FileInfo _fileInfo;
	memset(&_fileInfo, 0, sizeof(FileInfo));

	/*****************************���¼�¼**************************************/
	//�������� : 2010.03.08
	//����ԭ�� : ����GetSize�ӿ�
	//	CFileModel _fileModel(_strFilePath);
	_fileInfo._fileSize = CFileModel::GetSize(_strFilePath);
	/***************************************************************************/

	CFile file;
	if (file.Open(_strFilePath,CFile::modeRead))
	{
//		_fileInfo._size			= file.GetLength();
		CString _strFileName	= file.GetFileName();
		memcpy(_fileInfo._name, _strFileName, _strFileName.GetLength());
		_model->SetFilePath(_strFilePath);		//����Ϣ���浽ģ����
		_model->SetFileInfo(_fileInfo);
		file.Close();
	}
}

