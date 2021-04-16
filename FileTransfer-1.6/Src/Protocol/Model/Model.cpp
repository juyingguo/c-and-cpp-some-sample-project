/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : Model.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * ����ԭ�� : ������������ΪSize
 * �ļ�˵�� : implementation of the CModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Model.h"
#include "ConfigFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/***************************************************************************/
/************************ȫ�ֵ��̺߳�����ڵ�*******************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : ConnectProcessThread
 * �������� : ����TCP���Ӵ��������ڵ�
 * ��ڲ��� : LPVOID lpParam, ģ�Ͷ���ָ��
 * ���ڲ��� : TRUE
 ---------------------------------------------------------------------------*/
UINT ConnectProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->ConnectProcess();
	return TRUE;
}
/*---------------------------------------------------------------------------
 * �������� : AcceptProcessThread
 * �������� : ����TCP���Ӵ��������ڵ�
 * ��ڲ��� : LPVOID lpParam, ģ�Ͷ���ָ��
 * ���ڲ��� : TRUE
 ---------------------------------------------------------------------------*/
UINT AcceptProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->AcceptProcess();
	return TRUE;
}
/*---------------------------------------------------------------------------
 * �������� : MessageProcessThread
 * �������� : ��Ϣ�̴߳��������ڵ�
 * ��ڲ��� : LPVOID lpParam, ģ�Ͷ���ָ��
 * ���ڲ��� : TRUE
 ---------------------------------------------------------------------------*/
UINT MessageProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->MessageProcess();
	return TRUE;
}
/*---------------------------------------------------------------------------
 * �������� : TransferProcessThread
 * �������� : �ļ������̴߳��������ڵ�
 * ��ڲ��� : LPVOID lpParam, ģ�Ͷ���ָ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
UINT TransferProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->TransferProcess();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModel::CModel()
{
	
}

CModel::~CModel()
{

}
/***************************************************************************/
/************************���¶���CModel�����ṩ�Ľӿ�***********************/
/***************************************************************************/

/************************CModel���Ե�get��set����***************************/

int CModel::GetState()
{
	return this->_state;
}

CString CModel::GetAddress()
{
	return this->_strAddress;
}

void CModel::SetRemoteAddr(CString _strRemoteAddr)
{
	this->_strRemoteAddr = _strRemoteAddr;
}

CString CModel::GetRemoteAddr()
{
	return this->_strRemoteAddr;
}

void CModel::SetFilePath(CString _strFilePath)
{
	this->_strFilePath = _strFilePath;
}

void CModel::SetFileInfo(FileInfo _fileInfo)
{
	this->_fileInfo = _fileInfo;
}

FileInfo CModel::GetFileInfo()
{
	return this->_fileInfo;
}

void CModel::SetOffset(Size _offset)
{
	this->_offset = _offset;
}

Size CModel::GetOffset()
{
	return this->_offset;
}

/************************CModel�Ĺ�������**********************************/

/*---------------------------------------------------------------------------
 * �������� : BeginConnectThread
 * �������� : ����TCP���Ӵ����߳�
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::BeginConnectThread()
{
	AfxBeginThread(ConnectProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * �������� : BeginAcceptThread
 * �������� : ��������TCP���Ӵ����߳�
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::BeginAcceptThread()
{
	AfxBeginThread(AcceptProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * �������� : BeginMessageThread
 * �������� : ������Ϣ�����߳�
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::BeginMessageThread()
{
	AfxBeginThread(MessageProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * �������� : BeginTransferThread
 * �������� : �����ļ������߳�
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::BeginTransferThread()
{
	AfxBeginThread(TransferProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * �������� : MessageProcess
 * �������� : ��Ϣ������̣��˴�������Ϣ���������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::MessageProcess()
{
	InitializeCriticalSection(&g_csMessage);
	while (TRUE)
	{
		if (_state == INIT)						//�Ͽ����Ӻ󣬹ر���Ϣ�����߳�
		{
			AfxEndThread(0);
		}
		else
		{
			EnterCriticalSection(&g_csMessage);
			int _length = recv(_connectedSock, (char *)(&_buffer), sizeof(Message), 0);
			LeaveCriticalSection(&g_csMessage);
			if (_length != SOCKET_ERROR)
			{
				//message handle
				Handle();
			}
			else
			{
				Sleep(1);
			}
		}	
	}
}
/*---------------------------------------------------------------------------
 * �������� : InterruptTransfer
 * �������� : �ж��ļ�������̵Ĵ���
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::InterruptTransfer()
{
	Message _msg;
	memset(&_msg, 0, sizeof(Message));
	SetMessageType(_msg, INTERRUPT_TRANSFER);
	
	if (Send(_msg) == SOCKET_ERROR)
	{
		Sleep(1);
	}
	else
	{
		_state = CONNECTED;
		Reset();
		::SendMessage(m_hWnd, INTERRUPT_TRANSFER, 0, 0);
	}
}
/*---------------------------------------------------------------------------
 * �������� : CloseConnect
 * �������� : (����)�ر����ӵĴ������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::CloseConnect()
{
	Message _msg;
	memset(&_msg, 0, sizeof(Message));
	SetMessageType(_msg, CLOSE_CONNECT);
	if (Send(_msg) == SOCKET_ERROR)
	{
		Sleep(1);
	}
	else
	{
		Sleep(MAX_ELAPSE);		//����Ҫ��ʱ�������ȴ���Ϣ���ͳ�ȥ֮���ٹر�����
		CloseConnectProcess();
	}
}

/************************�ӳٵ�����ʵ�ֵķ���******************************/

void CModel::ConnectProcess()
{
	
}

void CModel::AcceptProcess()
{
	
}

int CModel::TransferFileInfo()
{
	return TRUE;
}

void CModel::TransferProcess()
{
	
}

/***************************************************************************/
/*****************************CModel���ڲ�����******************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : Init
 * �������� : ��ʼ��ģ�͵������������
 * ��ڲ��� : HWND m_hWnd, ��ͼ���
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::Init(HWND m_hWnd)
{
	_state = INIT;
	this->m_hWnd = m_hWnd;
	_strAddress = GetHostIPAddress();
	Reset();
	CConfigFile _file;
	if (_file.Load() == FALSE)
	{
		AfxMessageBox("���������ļ�ʧ�ܣ�");
		exit(0);
	}
	InitPort(_file);
	InitMaxConnect(_file);
	InitRemoteAddr(_file);
	InitSock();
}

/*---------------------------------------------------------------------------
 * �������� : InitPort
 * �������� : ��ʼ��server����Ϣ�����Ķ˿�
 * ��ڲ��� : CConfigFile _file, �����ļ�����
 * ���ڲ��� : ��
 * �������� : 2010.03.08
 * ����ԭ�� : �����Ϸ����жϷ�ʽ
 ---------------------------------------------------------------------------*/
void CModel::InitPort(CConfigFile _file)
{
	CString _strTemp = _file.GetValue(INDEX_SERVER_PORT);
	if (_file.Judge(_strTemp, MAX_PORT, MIN_PORT) == FALSE)
	{
		AfxMessageBox("�˿����ô���");
		exit(0);
	}
	_port = atoi(_strTemp);
}
/*---------------------------------------------------------------------------
 * �������� : InitSocket
 * �������� : ��ʼ��SOCKET
 * ��ڲ��� : SOCKET & _sock, ����ʼ�����׽��ֶ���
 * ���ڲ��� : ��ʼ���ɹ�����TRUE�����򷵻�FALSE
 ---------------------------------------------------------------------------*/
int CModel::InitSocket(SOCKET & _sock)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		return FALSE;
	}
	
	if ( (_sock = socket(AF_INET,SOCK_STREAM,0)) == SOCKET_ERROR )
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------
 * �������� : GetHostIPAddress
 * �������� : ��ȡ���ص�IP��ַ
 * ��ڲ��� : ��
 * ���ڲ��� : ���ص�IP��ַ
 ---------------------------------------------------------------------------*/
CString CModel::GetHostIPAddress()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[MAX_BUF_LEN];
	CString ip = _T("");
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2,0);
	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		if( gethostname( name, sizeof(name) ) == 0 )
		{
			if((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa(*(struct in_addr * ) *hostinfo->h_addr_list);
			}   
		}
		WSACleanup();
	}
	return ip;
}
/*---------------------------------------------------------------------------
 * �������� : Reset
 * �������� : ����ģ�͵Ĳ�������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::Reset()
{
	_offset	= 0;
	_strFilePath = _T("");
	memset(&_buffer, 0, sizeof(Message));
}

/*---------------------------------------------------------------------------
 * �������� : IsValid
 * �������� : �ж��ַ�����ʽ���������Ƿ�Ϸ�
 * ��ڲ��� : CString _strConfig, ������
 * ���ڲ��� : �Ϸ�����TRUE, ���򷵻�FALSE
 ---------------------------------------------------------------------------*/
//DEL int CModel::IsValid(int _configIndex, CString _strConfig)
//DEL {
//DEL 	//��������������ķ�Χ
//DEL 	int _min, _max;
//DEL 	switch (_configIndex)
//DEL 	{
//DEL 	case INDEX_SERVER_PORT:
//DEL 		_min = MIN_PORT;
//DEL 		_max = MAX_PORT;
//DEL 		break;
//DEL 	case INDEX_MAX_CONNECT:
//DEL 		_min = MIN_CONNECT;
//DEL 		_max = MAX_CONNECT;
//DEL 		break;
//DEL 	default:
//DEL 		return FALSE;
//DEL 	}
//DEL 	//Ȼ���ж��������ǲ��ǺϷ�������
//DEL 	for (int i = 0; i < _strConfig.GetLength(); i++)
//DEL 	{
//DEL 		char _cTemp = _strConfig.GetAt(i);
//DEL 		int _nAscii = (int)(_cTemp);
//DEL 		if (_nAscii < 0x30 || _nAscii > 0x39)
//DEL 		{
//DEL 			return FALSE;
//DEL 		}
//DEL 	}
//DEL 	//����ж�������ķ�Χ�Ƿ�Ϸ�
//DEL 	int _nTemp = atoi(_strConfig);
//DEL 	if (_nTemp < _min || _nTemp > _max)
//DEL 	{
//DEL 		return FALSE;
//DEL 	}
//DEL 	return TRUE;
//DEL }

/*---------------------------------------------------------------------------
 * �������� : Send
 * �������� : ������Ϣ
 * ��ڲ��� : Message &_msg, �����͵���Ϣ����
 * ���ڲ��� : ���ͳɹ�����ʵ�ʷ��͵���Ϣ���ȣ�����ʧ�ܷ���SOCKET_ERROR
 ---------------------------------------------------------------------------*/
int CModel::Send(Message &_msg)
{
	return send(_connectedSock, (const char *)(&_msg), sizeof(Message), 0);
}

/*---------------------------------------------------------------------------
 * �������� : TransferOver
 * �������� : ��������Ĵ�������������������Ҳ�����Ǵ����ж�
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::TransferOver()
{
	Reset();
	_state = CONNECTED;
	::SendMessage(m_hWnd, TRANSFER_OVER, 0, 0);
}
/*---------------------------------------------------------------------------
 * �������� : CloseConnectProcess
 * �������� : �ر�TCP���ӵ�ʵ�ʴ������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CModel::CloseConnectProcess()
{
	closesocket(_connectedSock);
	//memset(&_connectedSock, 0, sizeof(SOCKET));
	WSACleanup();
	InitSock();
	//InitSocket(_connectedSock);
	_state = INIT;
	Reset();
	::SendMessage(m_hWnd, CLOSE_CONNECT, 0, 0);
}

/************************�������ӳٵ�������ʵ�ֵķ���***********************/

void CModel::InitMaxConnect(CConfigFile _file)
{
	
}

void CModel::InitRemoteAddr(CConfigFile _file)
{
	
}

void CModel::InitSock()
{
	
}

void CModel::Handle()
{
	
}

void CModel::SetMessageType(Message &_msg, MessageType _messgaeType)
{

}
