/*******************************版权信息*************************************
 * 文件名称 : Model.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整数据类型为Size
 * 文件说明 : implementation of the CModel class.
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
/************************全局的线程函数入口点*******************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : ConnectProcessThread
 * 函数功能 : 建立TCP连接处理过程入口点
 * 入口参数 : LPVOID lpParam, 模型对象指针
 * 出口参数 : TRUE
 ---------------------------------------------------------------------------*/
UINT ConnectProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->ConnectProcess();
	return TRUE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : AcceptProcessThread
 * 函数功能 : 接收TCP连接处理过程入口点
 * 入口参数 : LPVOID lpParam, 模型对象指针
 * 出口参数 : TRUE
 ---------------------------------------------------------------------------*/
UINT AcceptProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->AcceptProcess();
	return TRUE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : MessageProcessThread
 * 函数功能 : 消息线程处理过程入口点
 * 入口参数 : LPVOID lpParam, 模型对象指针
 * 出口参数 : TRUE
 ---------------------------------------------------------------------------*/
UINT MessageProcessThread(LPVOID lpParam)
{
	CModel * _model = (CModel *)(lpParam);
	_model->MessageProcess();
	return TRUE;
}
/*---------------------------------------------------------------------------
 * 函数名称 : TransferProcessThread
 * 函数功能 : 文件传输线程处理过程入口点
 * 入口参数 : LPVOID lpParam, 模型对象指针
 * 出口参数 : 无
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
/************************以下定义CModel对外提供的接口***********************/
/***************************************************************************/

/************************CModel属性的get和set方法***************************/

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

/************************CModel的公共方法**********************************/

/*---------------------------------------------------------------------------
 * 函数名称 : BeginConnectThread
 * 函数功能 : 开启TCP连接处理线程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::BeginConnectThread()
{
	AfxBeginThread(ConnectProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * 函数名称 : BeginAcceptThread
 * 函数功能 : 开启接受TCP连接处理线程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::BeginAcceptThread()
{
	AfxBeginThread(AcceptProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * 函数名称 : BeginMessageThread
 * 函数功能 : 开启消息处理线程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::BeginMessageThread()
{
	AfxBeginThread(MessageProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * 函数名称 : BeginTransferThread
 * 函数功能 : 开启文件传输线程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::BeginTransferThread()
{
	AfxBeginThread(TransferProcessThread, this, THREAD_PRIORITY_NORMAL);
}
/*---------------------------------------------------------------------------
 * 函数名称 : MessageProcess
 * 函数功能 : 消息处理过程，此处控制消息处理的流程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::MessageProcess()
{
	InitializeCriticalSection(&g_csMessage);
	while (TRUE)
	{
		if (_state == INIT)						//断开连接后，关闭消息监听线程
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
 * 函数名称 : InterruptTransfer
 * 函数功能 : 中断文件传输过程的处理
 * 入口参数 : 无
 * 出口参数 : 无
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
 * 函数名称 : CloseConnect
 * 函数功能 : (本地)关闭连接的处理过程
 * 入口参数 : 无
 * 出口参数 : 无
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
		Sleep(MAX_ELAPSE);		//必须要延时操作，等待消息发送出去之后再关闭连接
		CloseConnectProcess();
	}
}

/************************延迟到子类实现的方法******************************/

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
/*****************************CModel的内部方法******************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : Init
 * 函数功能 : 初始化模型的所有相关属性
 * 入口参数 : HWND m_hWnd, 视图句柄
 * 出口参数 : 无
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
		AfxMessageBox("载入配置文件失败！");
		exit(0);
	}
	InitPort(_file);
	InitMaxConnect(_file);
	InitRemoteAddr(_file);
	InitSock();
}

/*---------------------------------------------------------------------------
 * 函数名称 : InitPort
 * 函数功能 : 初始化server端消息监听的端口
 * 入口参数 : CConfigFile _file, 配置文件对象
 * 出口参数 : 无
 * 更新日期 : 2010.03.08
 * 更新原因 : 调整合法性判断方式
 ---------------------------------------------------------------------------*/
void CModel::InitPort(CConfigFile _file)
{
	CString _strTemp = _file.GetValue(INDEX_SERVER_PORT);
	if (_file.Judge(_strTemp, MAX_PORT, MIN_PORT) == FALSE)
	{
		AfxMessageBox("端口配置错误！");
		exit(0);
	}
	_port = atoi(_strTemp);
}
/*---------------------------------------------------------------------------
 * 函数名称 : InitSocket
 * 函数功能 : 初始化SOCKET
 * 入口参数 : SOCKET & _sock, 待初始化的套接字对象
 * 出口参数 : 初始化成功返回TRUE，否则返回FALSE
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
 * 函数名称 : GetHostIPAddress
 * 函数功能 : 获取本地的IP地址
 * 入口参数 : 无
 * 出口参数 : 本地的IP地址
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
 * 函数名称 : Reset
 * 函数功能 : 重置模型的部分属性
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::Reset()
{
	_offset	= 0;
	_strFilePath = _T("");
	memset(&_buffer, 0, sizeof(Message));
}

/*---------------------------------------------------------------------------
 * 函数名称 : IsValid
 * 函数功能 : 判断字符串形式的配置项是否合法
 * 入口参数 : CString _strConfig, 配置项
 * 出口参数 : 合法返回TRUE, 否则返回FALSE
 ---------------------------------------------------------------------------*/
//DEL int CModel::IsValid(int _configIndex, CString _strConfig)
//DEL {
//DEL 	//首先设置配置项的范围
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
//DEL 	//然后判断配置项是不是合法的数字
//DEL 	for (int i = 0; i < _strConfig.GetLength(); i++)
//DEL 	{
//DEL 		char _cTemp = _strConfig.GetAt(i);
//DEL 		int _nAscii = (int)(_cTemp);
//DEL 		if (_nAscii < 0x30 || _nAscii > 0x39)
//DEL 		{
//DEL 			return FALSE;
//DEL 		}
//DEL 	}
//DEL 	//最后判断配置项的范围是否合法
//DEL 	int _nTemp = atoi(_strConfig);
//DEL 	if (_nTemp < _min || _nTemp > _max)
//DEL 	{
//DEL 		return FALSE;
//DEL 	}
//DEL 	return TRUE;
//DEL }

/*---------------------------------------------------------------------------
 * 函数名称 : Send
 * 函数功能 : 发送消息
 * 入口参数 : Message &_msg, 待发送的消息内容
 * 出口参数 : 发送成功返回实际发送的消息长度，发送失败返回SOCKET_ERROR
 ---------------------------------------------------------------------------*/
int CModel::Send(Message &_msg)
{
	return send(_connectedSock, (const char *)(&_msg), sizeof(Message), 0);
}

/*---------------------------------------------------------------------------
 * 函数名称 : TransferOver
 * 函数功能 : 传输结束的处理，可能是正常结束，也可能是传输中断
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CModel::TransferOver()
{
	Reset();
	_state = CONNECTED;
	::SendMessage(m_hWnd, TRANSFER_OVER, 0, 0);
}
/*---------------------------------------------------------------------------
 * 函数名称 : CloseConnectProcess
 * 函数功能 : 关闭TCP连接的实际处理过程
 * 入口参数 : 无
 * 出口参数 : 无
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

/************************以下是延迟到子类中实现的方法***********************/

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
