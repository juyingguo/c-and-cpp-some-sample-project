/*******************************版权信息*************************************
 * 文件名称 : FileTransferClientDlg.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.07
 * 文件说明 : header file
 ****************************************************************************/

#if !defined(AFX_FILETRANSFERCLIENTDLG_H__7F3FE5F8_5185_430F_AC76_C2562D1105F4__INCLUDED_)
#define AFX_FILETRANSFERCLIENTDLG_H__7F3FE5F8_5185_430F_AC76_C2562D1105F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileTransferClientDlg dialog

class CModel;
class CControl;

/*
 * 类的名称 : CFileTransferClientDlg
 * 基本用途 : 提供应用程序视图层的相关属性和接口
 */

class CFileTransferClientDlg : public CDialog
{
// Construction
public:
	CFileTransferClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileTransferClientDlg)
	enum { IDD = IDD_FILETRANSFERCLIENT_DIALOG };
	CIPAddressCtrl	m_ctlAddress;
	CProgressCtrl	m_ctlProgress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTransferClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	/***************************************************************************/
	/********************以下定义CFileTransferClientDlg的内部方法***************/
	/***************************************************************************/
	void Init();													//初始化视图基本信息
	HICON m_hIcon;
	/***************以下是CFileTransferClientDlg的消息映射**********************/
	// Generated message map functions
	//{{AFX_MSG(CFileTransferClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowzer();
	afx_msg void OnConnect();
	afx_msg void OnAbout();
	afx_msg void OnInterrupt();
	afx_msg void OnLoadHelp();
	afx_msg void OnCloseConnect();
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);		//传输进度更新的消息映射
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);		//最小化到托盘消息映射
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	/***************************************************************************/
	/******************以下是CFileTransferClientDlg的基本属性*******************/
	/***************************************************************************/
	CModel * _model;												//传输模型对象
	CControl * _control;											//视图控制对象
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANSFERCLIENTDLG_H__7F3FE5F8_5185_430F_AC76_C2562D1105F4__INCLUDED_)
