/*******************************版权信息*************************************
 * 文件名称 : ServerModel.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : interface for the CServerModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERMODEL_H__4DBF5C17_82F9_4E5A_B91D_97615CA2FD49__INCLUDED_)
#define AFX_SERVERMODEL_H__4DBF5C17_82F9_4E5A_B91D_97615CA2FD49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Model.h"

/*
 * 类的名称 : CServerModel
 * 基本用途 : 实现基本抽象模型的接口，提供server模型的基本功能
 */

class CServerModel : public CModel  
{
public:
	/***************************************************************************/
	/*****************以下是CServerModel对外提供的接口**************************/
	/***************************************************************************/

	/********************CServerModel的公共方法*********************************/
	void AcceptProcess();								//接收连接的处理过程
	int TransferFileInfo();								//传输文件信息
	void TransferProcess();								//传输文件的过程
	
	CServerModel(HWND m_hWnd);
	virtual ~CServerModel();
protected:
	/***************************************************************************/
	/*****************以下是CServerModel的内部方法******************************/
	/***************************************************************************/
	void InitSock();									//初始化套接字
	void InitMaxConnect(CConfigFile _file);				//初始化最大连接数
	void Handle();										//消息处理过程
	void SetMessageType(Message &_msg, MessageType _messgaeType);
														//设置消息的类型
private:
	/***************************************************************************/
	/*****************以下定义CServerModel的基本属性****************************/
	/***************************************************************************/
	SOCKET _localSock;									//本地套接字
	int _maxConnect;									//最大连接数
};

#endif // !defined(AFX_SERVERMODEL_H__4DBF5C17_82F9_4E5A_B91D_97615CA2FD49__INCLUDED_)
