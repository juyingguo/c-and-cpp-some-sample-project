/*******************************版权信息*************************************
 * 文件名称 : Control.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.04
 * 更新原因 : 调整数据类型为Size
 * 文件说明 : interface for the CControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROL_H__855FEBE6_64F6_4F3E_B972_5AE55046BA3D__INCLUDED_)
#define AFX_CONTROL_H__855FEBE6_64F6_4F3E_B972_5AE55046BA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

class CModel;

/*
 * 类的名称 : CControl
 * 基本用途 : 提供视图信息的控制方法
 */

class CControl  
{
public:
	
	/***************************************************************************/
	/*********************以下是CControl对外提供的接口**************************/
	/***************************************************************************/
	int JudgeMessage(UINT message);							//判断消息类型
	void LoadHelp();										//载入帮助文件
	int GetProgress();										//获取传输进度
	void UpdateControl(CDialog * _pView);					//更新传输时的控件信息
	void ToTray();											//最小化到托盘
	void DeleteTray();										//删除托盘
	CString SizeToString(Size _size, int _radix);			//提供转化Size为CString的接口
	/*********************以下方法延迟到子类中实现******************************/
	virtual void Browzer();									//设置待传输的文件路径
	virtual int IsValidPath();								//判断路径是否合法
	virtual void ConnectProcess(CString _strRemoteAddr);	//建立TCP连接的处理，client端实现
	virtual void BeginTransfer();							//开启文件传输的流程控制
	virtual void MessageProcess(UINT message);				//视图消息的处理过程
	
	CControl();
	virtual ~CControl();
protected:
	/***************************************************************************/
	/*********************以下是CControl的内部方法******************************/
	/***************************************************************************/
	void Init();											//初始化视图状态
	void InitStateMap();									//初始化状态字符信息映射表
	void ResetControl(CDialog * _pView);					//重置控件状态
	void UpdateView();										//更新视图信息
	CString GetSpeed();										//获取实时传输速度信息
	CString GetRemainedTime();								//获取剩余时间
	/********************以下方法延迟到子类实现*********************************/
	virtual void InitControlID();							//初始化控件资源号
	virtual void InitControlStateMap();						//初始化控件状态映射表
	virtual void InitFilePath();							//设置文件路径，client端实现
	virtual void ResetView();								//重置视图信息
	virtual void SetFileInfo();								//设置模型的文件信息
protected:
	/***************************************************************************/
	/*******************以下是CControl的属性定义********************************/
	/***************************************************************************/
	CModel * _model;										//模型对象指针
	HWND m_hWnd;											//视图句柄
	int _uID;												//视图资源号

	int _stateID;											//状态控件的资源号，子类实例化
	CString _strStateMap[STATE_NUM];						//状态字符串映射表，子类实例化
	
	int _controlID[MAX_CONTROL_NUM];						//控件资源号集合，子类实例化
	int _controlNum;										//需要改变状态的控件数量，子类实例化
	int _ctlStateMap[MAX_CONTROL_NUM][STATE_NUM];			//控件状态映射表，子类实例化

	int _speedID;											//速度控件的资源号，子类实例化
	int _remainedTimeID;									//剩余时间控件对应的资源号，子类实例化
	//////////////////////////2010.03.04更新，调整数据类型///////////////////////
	Size _startOffset;										//起始位置，用于速度更新
	Size _endOffset;										//终止位置，用于速度更新
	/////////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_CONTROL_H__855FEBE6_64F6_4F3E_B972_5AE55046BA3D__INCLUDED_)
