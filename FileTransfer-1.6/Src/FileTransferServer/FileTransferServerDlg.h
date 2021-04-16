/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileTransferServerDlg.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : header file
 ****************************************************************************/

#if !defined(AFX_FILETRANSFERSERVERDLG_H__9619A708_F415_4085_81EB_2C00F61E9388__INCLUDED_)
#define AFX_FILETRANSFERSERVERDLG_H__9619A708_F415_4085_81EB_2C00F61E9388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileTransferServerDlg dialog

/*
 * ������� : CFileTransferServerDlg
 * ������; : �ṩӦ�ó�����ͼ���������Ժͽӿ�
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
	/**********************���¶���CFileTransferServerDlg���ڲ�����*************/
	/***************************************************************************/
	void Init();													//��ʼ����ͼ����
	HICON m_hIcon;
	/***********************��������ͼ����Ϣӳ��********************************/
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
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);		//���ȸ���ӳ��
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);		//��С����������Ϣӳ��
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	/***************************************************************************/
	/*********************���¶���CFileTransferServerDlg������******************/
	/***************************************************************************/
	CModel * _model;								//ģ�Ͷ���ָ��
	CControl * _control;							//���ƶ���ָ��
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANSFERSERVERDLG_H__9619A708_F415_4085_81EB_2C00F61E9388__INCLUDED_)
