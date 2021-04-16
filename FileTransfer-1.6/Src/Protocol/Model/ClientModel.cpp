/*******************************版权信息*************************************
 * 文件名称 : ClientModel.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : implementation of the CClientModel class.
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
/****************以下定义CClientModel对外提供的方法*************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : ConnectProcess
 * 函数功能 : 连接服务器
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientModel::ConnectProcess()
{
	SOCKADDR_IN _sin;
	_sin.sin_family			= AF_INET;
	_sin.sin_port			= htons(_port);
	_sin.sin_addr.s_addr	= inet_addr(_strRemoteAddr);//inet_addr(_strAddress);

	if (connect(_connectedSock, (SOCKADDR *)&_sin, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		AfxMessageBox("连接失败！");
	}
	else
	{
		_state = CONNECTED;
		::SendMessage(m_hWnd, CONNECT_SUCCESS, 0, 0);
		BeginMessageThread();
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : TransferFileInfo
 * 函数功能 : 接收文件信息，返回确认
 * 入口参数 : 无
 * 出口参数 : 确认成功返回TRUE, 否则返回FALSE
 ---------------------------------------------------------------------------*/
int CClientModel::TransferFileInfo()
{
	Message _msg;
	memset(&_msg, 0, sizeof(Message));
	_msg._messageFromClient._message = RECV_FILE_INFO_ACK;
	_msg._messageFromClient._offset = _offset;		//发送文件的断点续传位置信息
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
			_state = TRANSFERRING;					//发送确认信息之后切换状态
		}
		return TRUE;
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : TransferProcess
 * 函数功能 : 实现具体的文件接收过程
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.08
 * 更新原因 : 调整文件模型的使用接口
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
		Size _fileSize = _offset + _size;			//递增式写入，保证能正确读取断点续传的位置

		//CFileModel * _fileModel = new CFileWriteModel(_strFilePath, _fileSize);

		//写一块文件，为提高效率，此处直接采用静态API
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

			if (_offset >= _fileInfo._fileSize)		//接收到文件的最后一块不需要发送确认信息
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
/****************以下定义CClientModel的内部方法*****************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : InitRemoteAddr
 * 函数功能 : 初始化服务器地址，利用配置文件读取
 * 入口参数 : CConfigFile _file, 配置文件
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientModel::InitRemoteAddr(CConfigFile _file)
{
	CString _strIP = _file.GetValue(INDEX_SERVER_ADDRESS);
	if (inet_addr(_strIP) == INADDR_NONE)
	{
		AfxMessageBox("服务器地址配置错误！");
		exit(0);
	}
	_strRemoteAddr = _strIP;
}
/*---------------------------------------------------------------------------
 * 函数名称 : InitSock
 * 函数功能 : 实现基类的虚方法，对于Client，要初始化的socket是_connectedSock
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientModel::InitSock()
{
	if (InitSocket(_connectedSock) == FALSE)
	{
		AfxMessageBox("初始化套接字失败!");
		exit(0);
	}
}
/*---------------------------------------------------------------------------
 * 函数名称 : Handle
 * 函数功能 : 实现client端的消息处理过程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientModel::Handle()
{
	MessageType _msg = _buffer._messageFromServer._messageType;
	switch (_msg)
	{
	case SEND_FILE_INFO_REQ:
		memset(&_fileInfo, 0, sizeof(_fileInfo));
		_fileInfo = _buffer._messageFromServer._data._fileInfo;		//保存文件信息
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
* 函数名称 : SetMessageType
* 函数功能 : 设置消息的类型
* 入口参数 : Message &_msg, 待设置的消息; MessageType _messgaeType, 消息类型
* 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CClientModel::SetMessageType(Message &_msg, MessageType _messgaeType)
{
	_msg._messageFromClient._message = _messgaeType;
}
