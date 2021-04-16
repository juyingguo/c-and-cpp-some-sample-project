/*******************************版权信息*************************************
 * 文件名称 : Model.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.04
 * 更新日期 : 调整数据类型为Size
 * 文件说明 : interface for the CModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL_H__06C4436C_1F1D_424E_B543_0DC8B6AA2CFA__INCLUDED_)
#define AFX_MODEL_H__06C4436C_1F1D_424E_B543_0DC8B6AA2CFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"
#include "..\KERNEL\AppConfig.h"	// Added by ClassView

UINT ConnectProcessThread(LPVOID lpParam);				//建立TCP连接线程的处理
UINT AcceptProcessThread(LPVOID lpParam);				//接收连接的线程方法声明
UINT MessageProcessThread(LPVOID lpParam);				//全局的消息线程处理函数的声明
UINT TransferProcessThread(LPVOID lpParam);				//全局的文件传输线程处理函数的声明

/*
 * 类的名称 : CModel
 * 基本用途 : 定义文件传输的基本模型
 */

class CConfigFile;

class CModel  
{
public:
	
	
	/***************************************************************************/
	/************************以下定义CModel对外提供的接口***********************/
	/***************************************************************************/

	/************************CModel属性的get和set方法***************************/
	int GetState();

	CString GetAddress();

	void SetRemoteAddr(CString _strRemoteAddr);
	CString GetRemoteAddr();

	void SetFilePath(CString _strFilePath);

	void SetFileInfo(FileInfo _fileInfo);
	FileInfo GetFileInfo();

	void SetOffset(Size _offset);
	Size GetOffset();

	/************************CModel的公共方法**********************************/
	void BeginConnectThread();								//开启TCP连接线程
	void BeginAcceptThread();								//开启接收TCP连接线程
	void BeginMessageThread();								//开启消息处理线程
	void BeginTransferThread();								//开启传输线程
	void MessageProcess();									//消息处理过程
	void InterruptTransfer();								//中断传输
	void CloseConnect();									//关闭连接
	/************************延迟到子类实现的方法******************************/
	
	virtual void ConnectProcess();							//连接处理，client实现
	virtual void AcceptProcess();							//接收连接处理，server实现
	virtual int TransferFileInfo();							//传输文件信息
	virtual void TransferProcess();							//文件传输过程
	
	CModel();
	virtual ~CModel();
protected:
	/***************************************************************************/
	/*****************************CModel的内部方法******************************/
	/***************************************************************************/
	void Init(HWND m_hWnd);									//顶层初始化操作
	void InitPort(CConfigFile _file);						//初始化server端口
	int InitSocket(SOCKET & _sock);							//初始化socket
	CString GetHostIPAddress();								//获取本地地址
	void Reset();											//重置模型的数据
	int Send(Message & _msg);								//发送消息
	void TransferOver();									//传输结束的具体处理
	void CloseConnectProcess();								//关闭连接的具体处理
	/************************以下是延迟到子类中实现的方法***********************/
	virtual void InitMaxConnect(CConfigFile _file);			//初始化最大连接数，server端实现
	virtual void InitRemoteAddr(CConfigFile _file);			//初始化远程地址，client端实现
	virtual void InitSock();								//初始化套接字的所有相关内容
	virtual void Handle();									//实际的消息处理过程
	virtual void SetMessageType(Message &_msg, MessageType _messgaeType);
															//设置消息结构中消息类型部分
protected:
	
	/***************************************************************************/
	/*************************以下定义CModel的基本属性**************************/
	/***************************************************************************/
	HWND m_hWnd;											//视图层句柄，用于模型和视图的通信
	int _state;												//模型状态的标识
	SOCKET _connectedSock;									//建立TCP连接的SOCKET
	CString _strAddress;									//本地地址
	CString _strRemoteAddr;									//远地地址
	int _port;												//server端消息监听端口
	Message _buffer;										//接收消息的缓冲区
	
	CString _strFilePath;									//待传输的文件路径
	FileInfo _fileInfo;										//待传输的文件信息
	//long _offset;											//已传输的文件块数，用于断点续传
	//DWORD _offSet;										//已传输的文件大小，用于断点续传
	////////////2010.03.04更新，调整数据类型为Size///////////////////////////////
	Size _offset;											//已传输的文件大小，用于断点续传
	/////////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_MODEL_H__06C4436C_1F1D_424E_B543_0DC8B6AA2CFA__INCLUDED_)
