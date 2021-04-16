/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ClientModel.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : implementation of the CClientModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientModel.h"
#include "ConfigFile.h"
#include "FileWriteModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientModel::CClientModel(HWND m_hWnd)
{
	InitializeCriticalSection(&g_csMessage);
	InitializeCriticalSection(&g_csTransfer);
	Init(m_hWnd);
}

CClientModel::~CClientModel()
{

}

/***************************************************************************/
/****************���¶���CClientModel�����ṩ�ķ���*************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : ConnectProcess
 * �������� : ���ӷ�����
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientModel::ConnectProcess()
{
	SOCKADDR_IN _sin;
	_sin.sin_family			= AF_INET;
	_sin.sin_port			= htons(_port);
	_sin.sin_addr.s_addr	= inet_addr(_strRemoteAddr);//inet_addr(_strAddress);

	if (connect(_connectedSock, (SOCKADDR *)&_sin, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		AfxMessageBox("����ʧ�ܣ�");
	}
	else
	{
		_state = CONNECTED;
		::SendMessage(m_hWnd, CONNECT_SUCCESS, 0, 0);
		BeginMessageThread();
	}
}
/*---------------------------------------------------------------------------
 * �������� : TransferFileInfo
 * �������� : �����ļ���Ϣ������ȷ��
 * ��ڲ��� : ��
 * ���ڲ��� : ȷ�ϳɹ�����TRUE, ���򷵻�FALSE
 ---------------------------------------------------------------------------*/
int CClientModel::TransferFileInfo()
{
	Message _msg;
	memset(&_msg, 0, sizeof(Message));
	_msg._messageFromClient._message = RECV_FILE_INFO_ACK;
	_msg._messageFromClient._offset = _offset;		//�����ļ��Ķϵ�����λ����Ϣ
	if (Send(_msg) == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
		if (_offset >= _fileInfo._fileSize)
		{
			TransferOver();
		}
		else
		{
			_state = TRANSFERRING;					//����ȷ����Ϣ֮���л�״̬
		}
		return TRUE;
	}
}
/*---------------------------------------------------------------------------
 * �������� : TransferProcess
 * �������� : ʵ�־�����ļ����չ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.08
 * ����ԭ�� : �����ļ�ģ�͵�ʹ�ýӿ�
 ---------------------------------------------------------------------------*/
void CClientModel::TransferProcess()
{
	InitializeCriticalSection(&g_csTransfer);
	if ( (_offset < _fileInfo._fileSize) && (_fileInfo._fileSize > 0) )
	{
		if (_state != TRANSFERRING)
		{
			AfxEndThread(0);
		}

		DWORD _size = _buffer._messageFromServer._data._transferData._len;
		Size _fileSize = _offset + _size;			//����ʽд�룬��֤����ȷ��ȡ�ϵ�������λ��

		//CFileModel * _fileModel = new CFileWriteModel(_strFilePath, _fileSize);

		//дһ���ļ���Ϊ���Ч�ʣ��˴�ֱ�Ӳ��þ�̬API
		EnterCriticalSection(&g_csTransfer);
		//_size = _fileModel->Serialize(_buffer._messageFromServer._data._transferData._buf, _offset, _size);
		_size = CFileModel::Write(_strFilePath, _fileSize, _buffer._messageFromServer._data._transferData._buf, _offset, _size);
		LeaveCriticalSection(&g_csTransfer);

		//delete _fileModel;
		//_fileModel = NULL;

		if (_size != WRITE_ERROR)
		{
			EnterCriticalSection(&g_csTransfer);
			_offset = _offset + _size;
			LeaveCriticalSection(&g_csTransfer);

			if (_offset >= _fileInfo._fileSize)		//���յ��ļ������һ�鲻��Ҫ����ȷ����Ϣ
			{
				TransferOver();
				AfxEndThread(0);
			}
		}

		Message _msg;
		memset(&_msg, 0, sizeof(Message));
		_msg._messageFromClient._message = RECV_FILE_ACK;
		_msg._messageFromClient._offset = _offset;
		if (Send(_msg) == SOCKET_ERROR)
		{
			InterruptTransfer();
			AfxEndThread(0);
		}
		::SendMessage(m_hWnd, TRANSFERRING_FILE, 0, 0);
	}
}
/***************************************************************************/
/****************���¶���CClientModel���ڲ�����*****************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : InitRemoteAddr
 * �������� : ��ʼ����������ַ�����������ļ���ȡ
 * ��ڲ��� : CConfigFile _file, �����ļ�
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientModel::InitRemoteAddr(CConfigFile _file)
{
	CString _strIP = _file.GetValue(INDEX_SERVER_ADDRESS);
	if (inet_addr(_strIP) == INADDR_NONE)
	{
		AfxMessageBox("��������ַ���ô���");
		exit(0);
	}
	_strRemoteAddr = _strIP;
}
/*---------------------------------------------------------------------------
 * �������� : InitSock
 * �������� : ʵ�ֻ�����鷽��������Client��Ҫ��ʼ����socket��_connectedSock
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientModel::InitSock()
{
	if (InitSocket(_connectedSock) == FALSE)
	{
		AfxMessageBox("��ʼ���׽���ʧ��!");
		exit(0);
	}
}
/*---------------------------------------------------------------------------
 * �������� : Handle
 * �������� : ʵ��client�˵���Ϣ�������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientModel::Handle()
{
	MessageType _msg = _buffer._messageFromServer._messageType;
	switch (_msg)
	{
	case SEND_FILE_INFO_REQ:
		memset(&_fileInfo, 0, sizeof(_fileInfo));
		_fileInfo = _buffer._messageFromServer._data._fileInfo;		//�����ļ���Ϣ
		break;
	case SEND_FILE_REQ:
		BeginTransferThread();
		break;
	case INTERRUPT_TRANSFER:
		_state = CONNECTED;
		Reset();
		break;
	case CLOSE_CONNECT:
		CloseConnectProcess();
		return;		//
	}
	::SendMessage(m_hWnd, _msg, 0, 0);
}
/*---------------------------------------------------------------------------
* �������� : SetMessageType
* �������� : ������Ϣ������
* ��ڲ��� : Message &_msg, �����õ���Ϣ; MessageType _messgaeType, ��Ϣ����
* ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CClientModel::SetMessageType(Message &_msg, MessageType _messgaeType)
{
	_msg._messageFromClient._message = _messgaeType;
}
