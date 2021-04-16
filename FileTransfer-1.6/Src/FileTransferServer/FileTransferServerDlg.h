/*******************************版权信息*************************************
 * 文件名称 : FileTransferServerDlg.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : header file
 ****************************************************************************/

#if !defined(AFX_FILETRANSFERSERVERDLG_H__9619A708_F415_4085_81EB_2C00F61E9388__INCLUDED_)
#define AFX_FILETRANSFERSERVERDLG_H__9619A708_F415_4085_81EB_2C00F61E9388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileTransferServerDlg dialog

/*
 * 类的名称 : CFileTransferServerDlg
 * 基本用途 : 提供应用程序视图层的相关属性和接口
 */

class CModel;
class CControl;

class CFileTransferServerDlg : public CDialog
{
// Construction
public:
	CFileTransferServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileTransferServerDlg)
	enum { IDD = IDD_FILETRANSFERSERVER_DIALOG };
	CProgressCtrl	m_ctlProgress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTransferServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	/***************************************************************************/
	/**********************以下定义CFileTransferServerDlg的内部方法*************/
	/***************************************************************************/
	void Init();													//初始化视图属性
	HICON m_hIcon;
	/***********************以下是视图的消息映射********************************/
	// Generated message map functions
	//{{AFX_MSG(CFileTransferServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowzer();
	afx_msg void OnSend();
	afx_msg void OnAbout();
	afx_msg void OnInterrupt();
	afx_msg void OnLoadHelp();
	afx_msg void OnCloseConnect();
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);		//进度更新映射
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);		//最小化到托盘消息映射
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	/***************************************************************************/
	/*********************以下定义CFileTransferServerDlg的属性******************/
	/***************************************************************************/
	CModel * _model;								//模型对象指针
	CControl * _control;							//控制对象指针
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANSFERSERVERDLG_H__9619A708_F415_4085_81EB_2C00F61E9388__INCLUDED_)
