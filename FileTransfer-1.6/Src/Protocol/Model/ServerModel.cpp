/*******************************版权信息*************************************
 * 文件名称 : ServerModel.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : implementation of the CServerModel class.
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
/*****************以下是CServerModel对外提供的接口**************************/
/***************************************************************************/

/********************CServerModel的公共方法*********************************/

/*---------------------------------------------------------------------------
 * 函数名称 : AcceptProcess
 * 函数功能 : 接收TCP连接的处理过程
 * 入口参数 : 无
 * 出口参数 : 无
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
		_strRemoteAddr = inet_ntoa(_sin.sin_addr);		//保存远机地址
		_state = CONNECTED;								//改变模型状态
		::SendMessage(m_hWnd, CONNECT_SUCCESS, 0, 0);
		BeginMessageThread();							//开启消息监听线程
	}
}

/*---------------------------------------------------------------------------
 * 函数名称 : TransferFileInfo
 * 函数功能 : 实现抽象的文件信息传输接口，将文件基本信息发送给client
 * 入口参数 : 无
 * 出口参数 : 发送成功返回实际传输的大小，发送失败返回SOKCET_ERROR
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
 * 函数名称 : TransferProcess
 * 函数功能 : 实现文件传输的接口，对于Server来说，用于实现发送文件的功能
 * 入口参数 : 无
 * 出口参数 : 无
 * 更新日期 : 2010.03.08
 * 更新原因 : 设置读文件大小的控制逻辑; 调整文件模型的使用接口
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
		/*****************************更新记录**************************************/
		//更新日期 : 2010.03.08
		//更新原因 : 设置读文件大小的控制逻辑
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

		//读取一块文件, 为提高效率, 此处直接采用静态API
		EnterCriticalSection(&g_csTransfer);
		//_size = _fileModel->Serialize(_msg._messageFromServer._data._transferData._buf, _offset, _size);
		_size = CFileModel::Read(_strFilePath, _fileInfo._fileSize, _msg._messageFromServer._data._transferData._buf, _offset, _size);
		LeaveCriticalSection(&g_csTransfer);

		//delete _fileModel;
		//_fileModel = NULL;

		if (_size == READ_ERROR)
		{
			AfxMessageBox("读文件出错！");
			InterruptTransfer();
			AfxEndThread(0);
		}

		_msg._messageFromServer._messageType					= SEND_FILE_REQ;
		_msg._messageFromServer._data._transferData._len		= _size;
		_msg._messageFromServer._data._transferData._offset		= _offset;
																	//设置待发送的消息的内容

		if (Send(_msg) == SOCKET_ERROR)								//发送一块文件
		{
			AfxMessageBox("发送文件出错！");
			InterruptTransfer();
			AfxEndThread(0);
		}

		EnterCriticalSection(&g_csTransfer);
		_offset = _offset + _size;									//更新进度
		LeaveCriticalSection(&g_csTransfer);

		if (_offset >= _fileInfo._fileSize)
		{
			TransferOver();											//发送完毕
			AfxEndThread(0);
		}
		::SendMessage(m_hWnd, TRANSFERRING_FILE, 0, 0);				//发送完一块文件后提示更新视图
	}
}

/***************************************************************************/
/*****************以下是CServerModel实现的抽象类的内部接口******************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : InitSock
 * 函数功能 : 实现基类的虚方法，对于Server，要初始化的socket是_localSock
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerModel::InitSock()
{
	if (InitSocket(_localSock) == FALSE)
	{
		AfxMessageBox("初始化套接字失败!");
		exit(0);
	}
	SOCKADDR_IN _sin;
	_sin.sin_family			= AF_INET;
	_sin.sin_addr.s_addr	= htonl(INADDR_ANY);
	_sin.sin_port			= htons(_port);
	
	if ( bind(_localSock,(sockaddr *)&_sin,sizeof(sockaddr)) == SOCKET_ERROR )
	{
		AfxMessageBox("绑定错误!");
		exit(0);
	}
	if ( listen(_localSock, _maxConnect) < 0 )
	{
		AfxMessageBox("监听错误!");
		exit(0);
	}
	BeginAcceptThread();
}
/*---------------------------------------------------------------------------
 * 函数名称 : InitMaxConnect
 * 函数功能 : 初始化最大连接数
 * 入口参数 : CConfigFile _file, 配置文件对象
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerModel::InitMaxConnect(CConfigFile _file)
{
	CString _strTemp = _file.GetValue(INDEX_MAX_CONNECT);
	if (_file.Judge(_strTemp, MAX_CONNECT, MIN_CONNECT) == FALSE)
	{
		AfxMessageBox("最大连接数配置错误！");
		exit(0);
	}
	_maxConnect = atoi(_strTemp);
}
/*---------------------------------------------------------------------------
 * 函数名称 : Handle
 * 函数功能 : 进行server端的消息处理过程
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerModel::Handle()
{
	MessageType _msg = _buffer._messageFromClient._message;
	switch (_msg)
	{
	case RECV_FILE_INFO_ACK:
		_state = TRANSFERRING;								//接收到确认信息后切换状态
		_offset = _buffer._messageFromClient._offset;		//保存偏移值，提供断点续传
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
		_state = CONNECTED;									//结束传输线程
		Reset();
		break;
	case CLOSE_CONNECT:										//异地关闭连接
		_strRemoteAddr = _T("");							//首先要清除地址信息
		CloseConnectProcess();
		return;
	}
	::SendMessage(m_hWnd, _msg, 0, 0);
}
/*---------------------------------------------------------------------------
 * 函数名称 : SetMessageType
 * 函数功能 : 设置消息的类型
 * 入口参数 : Message &_msg, 待设置的消息; MessageType _messgaeType, 消息类型
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CServerModel::SetMessageType(Message &_msg, MessageType _messgaeType)
{
	_msg._messageFromServer._messageType = _messgaeType;
}
