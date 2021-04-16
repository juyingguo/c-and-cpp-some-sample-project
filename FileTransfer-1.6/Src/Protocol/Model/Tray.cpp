/*******************************版权信息*************************************
 * 文件名称 : Tray.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.24
 * 文件说明 : implementation of the CTray class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTray::CTray(HWND m_hWnd, int _uID)
{
	this->m_hWnd = m_hWnd;
	this->_uID = _uID;
}

CTray::~CTray()
{

}
/***************************************************************************/
/***********************以下是CTray类对外提供的统一接口*********************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : ToTray
 * 函数功能 : 最小化应用程序到托盘
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CTray::ToTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize				= (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd				= m_hWnd;
	nid.uID					= _uID;
	nid.uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage	= WM_SHOWTASK;						//自定义的消息名称
	nid.hIcon				= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(_uID));
	strcpy(nid.szTip,"程序名称");								//信息提示条
	Shell_NotifyIcon(NIM_ADD,&nid);								//在托盘区添加图标
	::ShowWindow(m_hWnd, SW_HIDE);								//隐藏主窗口
}
/*---------------------------------------------------------------------------
 * 函数名称 : DeleteTray
 * 函数功能 : 删除托盘图标
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CTray::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize				= (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd				= m_hWnd;
	nid.uID					= _uID;
	nid.uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage	= WM_SHOWTASK;						//自定义的消息名称
	nid.hIcon				= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(_uID));
	strcpy(nid.szTip,"程序名称");								//信息提示条为“计划任务提醒”
	Shell_NotifyIcon(NIM_DELETE,&nid);							//在托盘区删除图标
}
