/*******************************版权信息*************************************
 * 文件名称 : FileTransferServer.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.03
 * 文件说明 : main header file for the FILETRANSFERSERVER application
 ****************************************************************************/

#if !defined(AFX_FILETRANSFERSERVER_H__67BE5EB6_B675_4251_8324_88D3D232AE77__INCLUDED_)
#define AFX_FILETRANSFERSERVER_H__67BE5EB6_B675_4251_8324_88D3D232AE77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "../Protocol/Includes.h"				//包含协议的所有文件

/////////////////////////////////////////////////////////////////////////////
// CFileTransferServerApp:
// See FileTransferServer.cpp for the implementation of this class
//

/*
 * 类的名称 : CFileTransferServerApp
 * 基本用途 : 提供程序的入口点
 */
class CFileTransferServerApp : public CWinApp
{
public:
	CFileTransferServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTransferServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileTransferServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANSFERSERVER_H__67BE5EB6_B675_4251_8324_88D3D232AE77__INCLUDED_)
