/*******************************版权信息*************************************
 * 文件名称 : ClientModel.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : interface for the CClientModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTMODEL_H__D5F3703B_E221_4BF4_B5FA_CA03956161F9__INCLUDED_)
#define AFX_CLIENTMODEL_H__D5F3703B_E221_4BF4_B5FA_CA03956161F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Model.h"

/*
 * 类的名称 : CClientModel
 * 基本用途 : 实现基本抽象模型的接口，提供client模型的基本功能
 */

class CClientModel : public CModel  
{
public:
	/***************************************************************************/
	/****************以下定义CClientModel对外提供的方法*************************/
	/***************************************************************************/
	void ConnectProcess();											//建立TCP连接
	int TransferFileInfo();											//文件信息的传输
	void TransferProcess();											//文件传输过程
	
	CClientModel(HWND m_hWnd);
	virtual ~CClientModel();
protected:
	/***************************************************************************/
	/****************以下定义CClientModel的内部方法*****************************/
	/***************************************************************************/
	void InitRemoteAddr(CConfigFile _file);							//初始化server端地址
	void InitSock();												//初始化sock信息
	void Handle();													//消息处理过程
	void SetMessageType(Message &_msg, MessageType _messgaeType);	//设置消息类型
};

#endif // !defined(AFX_CLIENTMODEL_H__D5F3703B_E221_4BF4_B5FA_CA03956161F9__INCLUDED_)
