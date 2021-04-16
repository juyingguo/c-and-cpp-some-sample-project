/*******************************版权信息*************************************
 * 文件名称 : ServerControl.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : interface for the CServerControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERCONTROL_H__D7587300_1AE0_471D_86E7_57114C624834__INCLUDED_)
#define AFX_SERVERCONTROL_H__D7587300_1AE0_471D_86E7_57114C624834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileTransferServer.h"

class CFileTransferServerDlg;

/*
 * 类的名称 : CServerControl
 * 基本用途 : 实现抽象的CControl接口，提供Server端的视图信息的控制方法
 */

class CServerControl : public CControl  
{
public:
	/***************************************************************************/
	/*********************以下是CServerControl对外提供的接口********************/
	/***************************************************************************/
	void Browzer();										//设置文件路径
	int IsValidPath();									//判断路径的合法性
	void BeginTransfer();								//开启文件传输
	void MessageProcess(UINT message);					//视图消息处理过程
	
	CServerControl(int _uID, CFileTransferServerDlg * _pView, CModel * _model);
	virtual ~CServerControl();
protected:
	/***************************************************************************/
	/*********************以下是CServerControl的内部方法************************/
	/***************************************************************************/
	void InitControlID();								//初始化控件ID
	void InitControlStateMap();							//初始化控件状态映射表
	
	void ResetView();									//重置视图信息
	void SetFileInfo();									//设置文件信息
private:
	/***************************************************************************/
	/********************以下是CServerControl的属性定义*************************/
	/***************************************************************************/
	CFileTransferServerDlg * _pView;					//视图对象指针
};

#endif // !defined(AFX_SERVERCONTROL_H__D7587300_1AE0_471D_86E7_57114C624834__INCLUDED_)
