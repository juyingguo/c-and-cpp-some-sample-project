/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ServerModel.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : implementation of the CServerModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerModel.h"
#include "ConfigFile.h"
#include "FileReadModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerModel::CServerModel(HWND m_hWnd)
{
	InitializeCriticalSection(&g_csMessage);
	InitializeCriticalSection(&g_csTransfer);
	Init(m_hWnd);
}

CServerModel::~CServerModel()
{

}

/***************************************************************************/
/*****************������CServerModel�����ṩ�Ľӿ�**************************/
/***************************************************************************/

/********************CServerModel�Ĺ�������*********************************/

/*---------------------------------------------------------------------------
 * �������� : AcceptProcess
 * �������� : ����TCP���ӵĴ������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerModel::AcceptProcess()
{
	SOCKADDR_IN _sin;
	int _size = sizeof(SOCKADDR_IN);
	if( (_connectedSock = accept(_localSock, (sockaddr *)&_sin, &_size)) == SOCKET_ERROR )
	{
		Sleep(1);
	}
	else
	{
		_strRemoteAddr = inet_ntoa(_sin.sin_addr);		//����Զ����ַ
		_state = CONNECTED;								//�ı�ģ��״̬
		::SendMessage(m_hWnd, CONNECT_SUCCESS, 0, 0);
		BeginMessageThread();							//������Ϣ�����߳�
	}
}

/*---------------------------------------------------------------------------
 * �������� : TransferFileInfo
 * �������� : ʵ�ֳ�����ļ���Ϣ����ӿڣ����ļ�������Ϣ���͸�client
 * ��ڲ��� : ��
 * ���ڲ��� : ���ͳɹ�����ʵ�ʴ���Ĵ�С������ʧ�ܷ���SOKCET_ERROR
 ---------------------------------------------------------------------------*/
int CServerModel::TransferFileInfo()
{
	Message _msg;
	memset(&_msg, 0, sizeof(Message));
	_msg._messageFromServer._messageType = SEND_FILE_INFO_REQ;
	_msg._messageFromServer._data._fileInfo = _fileInfo;
	return Send(_msg);
}

/*---------------------------------------------------------------------------
 * �������� : TransferProcess
 * �������� : ʵ���ļ�����Ľӿڣ�����Server��˵������ʵ�ַ����ļ��Ĺ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 * �������� : 2010.03.08
 * ����ԭ�� : ���ö��ļ���С�Ŀ����߼�; �����ļ�ģ�͵�ʹ�ýӿ�
 ---------------------------------------------------------------------------*/
void CServerModel::TransferProcess()
{
	InitializeCriticalSection(&g_csTransfer);
	if ( (_offset < _fileInfo._fileSize) && (_fileInfo._fileSize > 0) )
	{
		if (_state != TRANSFERRING)
		{
			AfxEndThread(0);
		}
		/*****************************���¼�¼**************************************/
		//�������� : 2010.03.08
		//����ԭ�� : ���ö��ļ���С�Ŀ����߼�
		DWORD _size;
		if (_offset + BLOCK_SIZE > _fileInfo._fileSize)
		{
			_size = (DWORD)(_fileInfo._fileSize - _offset);
		}
		else
		{
			_size = BLOCK_SIZE;
		}
		/***************************************************************************/
		Message _msg;
		memset(&_msg, 0, sizeof(Message));
		
		//CFileModel * _fileModel;
		//_fileModel = new CFileReadModel(_strFilePath, _fileInfo._fileSize);

		//��ȡһ���ļ�, Ϊ���Ч��, �˴�ֱ�Ӳ��þ�̬API
		EnterCriticalSection(&g_csTransfer);
		//_size = _fileModel->Serialize(_msg._messageFromServer._data._transferData._buf, _offset, _size);
		_size = CFileModel::Read(_strFilePath, _fileInfo._fileSize, _msg._messageFromServer._data._transferData._buf, _offset, _size);
		LeaveCriticalSection(&g_csTransfer);

		//delete _fileModel;
		//_fileModel = NULL;

		if (_size == READ_ERROR)
		{
			AfxMessageBox("���ļ�����");
			InterruptTransfer();
			AfxEndThread(0);
		}

		_msg._messageFromServer._messageType					= SEND_FILE_REQ;
		_msg._messageFromServer._data._transferData._len		= _size;
		_msg._messageFromServer._data._transferData._offset		= _offset;
																	//���ô����͵���Ϣ������

		if (Send(_msg) == SOCKET_ERROR)								//����һ���ļ�
		{
			AfxMessageBox("�����ļ�����");
			InterruptTransfer();
			AfxEndThread(0);
		}

		EnterCriticalSection(&g_csTransfer);
		_offset = _offset + _size;									//���½���
		LeaveCriticalSection(&g_csTransfer);

		if (_offset >= _fileInfo._fileSize)
		{
			TransferOver();											//�������
			AfxEndThread(0);
		}
		::SendMessage(m_hWnd, TRANSFERRING_FILE, 0, 0);				//������һ���ļ�����ʾ������ͼ
	}
}

/***************************************************************************/
/*****************������CServerModelʵ�ֵĳ�������ڲ��ӿ�******************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : InitSock
 * �������� : ʵ�ֻ�����鷽��������Server��Ҫ��ʼ����socket��_localSock
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerModel::InitSock()
{
	if (InitSocket(_localSock) == FALSE)
	{
		AfxMessageBox("��ʼ���׽���ʧ��!");
		exit(0);
	}
	SOCKADDR_IN _sin;
	_sin.sin_family			= AF_INET;
	_sin.sin_addr.s_addr	= htonl(INADDR_ANY);
	_sin.sin_port			= htons(_port);
	
	if ( bind(_localSock,(sockaddr *)&_sin,sizeof(sockaddr)) == SOCKET_ERROR )
	{
		AfxMessageBox("�󶨴���!");
		exit(0);
	}
	if ( listen(_localSock, _maxConnect) < 0 )
	{
		AfxMessageBox("��������!");
		exit(0);
	}
	BeginAcceptThread();
}
/*---------------------------------------------------------------------------
 * �������� : InitMaxConnect
 * �������� : ��ʼ�����������
 * ��ڲ��� : CConfigFile _file, �����ļ�����
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerModel::InitMaxConnect(CConfigFile _file)
{
	CString _strTemp = _file.GetValue(INDEX_MAX_CONNECT);
	if (_file.Judge(_strTemp, MAX_CONNECT, MIN_CONNECT) == FALSE)
	{
		AfxMessageBox("������������ô���");
		exit(0);
	}
	_maxConnect = atoi(_strTemp);
}
/*---------------------------------------------------------------------------
 * �������� : Handle
 * �������� : ����server�˵���Ϣ�������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerModel::Handle()
{
	MessageType _msg = _buffer._messageFromClient._message;
	switch (_msg)
	{
	case RECV_FILE_INFO_ACK:
		_state = TRANSFERRING;								//���յ�ȷ����Ϣ���л�״̬
		_offset = _buffer._messageFromClient._offset;		//����ƫ��ֵ���ṩ�ϵ�����
		if (_offset >= _fileInfo._fileSize)
		{
			TransferOver();
		}
		else
		{
			BeginTransferThread();
		}
		break;
	case RECV_FILE_ACK:
		_offset = _buffer._messageFromClient._offset;
		BeginTransferThread();
		break;
	case INTERRUPT_TRANSFER:
		_state = CONNECTED;									//���������߳�
		Reset();
		break;
	case CLOSE_CONNECT:										//��عر�����
		_strRemoteAddr = _T("");							//����Ҫ�����ַ��Ϣ
		CloseConnectProcess();
		return;
	}
	::SendMessage(m_hWnd, _msg, 0, 0);
}
/*---------------------------------------------------------------------------
 * �������� : SetMessageType
 * �������� : ������Ϣ������
 * ��ڲ��� : Message &_msg, �����õ���Ϣ; MessageType _messgaeType, ��Ϣ����
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CServerModel::SetMessageType(Message &_msg, MessageType _messgaeType)
{
	_msg._messageFromServer._messageType = _messgaeType;
}
