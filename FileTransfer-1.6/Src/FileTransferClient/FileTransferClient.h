/*******************************版权信息*************************************
 * 文件名称 : FileTransferClient.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : main header file for the FILETRANSFERCLIENT application
 ****************************************************************************/

#if !defined(AFX_FILETRANSFERCLIENT_H__B1E9B7C7_C00F_4B0A_BA5E_0077E9BB3000__INCLUDED_)
#define AFX_FILETRANSFERCLIENT_H__B1E9B7C7_C00F_4B0A_BA5E_0077E9BB3000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "../Protocol/Includes.h"			//包含协议的所有文件

/////////////////////////////////////////////////////////////////////////////
// CFileTransferClientApp:
// See FileTransferClient.cpp for the implementation of this class
//

/*
 * 类的名称 : CFileTransferClientApp
 * 基本用途 : 提供应用程序的入口点
 */

class CFileTransferClientApp : public CWinApp
{
public:
	CFileTransferClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTransferClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileTransferClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANSFERCLIENT_H__B1E9B7C7_C00F_4B0A_BA5E_0077E9BB3000__INCLUDED_)
