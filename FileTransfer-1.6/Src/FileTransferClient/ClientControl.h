/*******************************版权信息*************************************
 * 文件名称 : ClientControl.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整因Size结构变更而变更的内容
 * 文件说明 : interface for the CClientControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTCONTROL_H__9FF30A33_D6A7_40A1_8D05_14B06567EFD1__INCLUDED_)
#define AFX_CLIENTCONTROL_H__9FF30A33_D6A7_40A1_8D05_14B06567EFD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileTransferClient.h"

class CFileTransferClientDlg;

/*
 * 类的名称 : CClientControl
 * 基本用途 : 实现抽象的CControl接口，提供Client端的视图信息的控制方法
 */

class CClientControl : public CControl  
{
public:
	/***************************************************************************/
	/*******************以下是CClientControl对外提供的接口**********************/
	/***************************************************************************/
	void Browzer();												//设置文件路径
	int IsValidPath();											//判断路径是否合法
	void ConnectProcess(CString _strRemoteAddr);				//建立TCP连接的处理过程
	void BeginTransfer();										//开启文件传输的流程控制
	void MessageProcess(UINT message);							//视图消息的处理
	CClientControl(int _uID, CFileTransferClientDlg * _pView, CModel * _model);
	virtual ~CClientControl();
protected:
	/***************************************************************************/
	/*******************以下是CClientControl的内部方法**************************/
	/***************************************************************************/
	void InitControlID();										//初始化控件ID
	void InitControlStateMap();									//初始化控件状态映射表
	void InitFilePath();										//初始化视图的文件路径信息
	void ResetView();											//重置视图信息
	
	void SetFileInfo();											//设置文件信息
	void SetFileSize();											//设置视图的文件大小信息
private:
	/***************************************************************************/
	/*******************以下是CClientControl的属性******************************/
	/***************************************************************************/
	CFileTransferClientDlg * _pView;							//视图对象指针
};

#endif // !defined(AFX_CLIENTCONTROL_H__9FF30A33_D6A7_40A1_8D05_14B06567EFD1__INCLUDED_)
