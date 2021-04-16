/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileTransferClientDlg.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : header file
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
 * ������� : CFileTransferClientDlg
 * ������; : �ṩӦ�ó�����ͼ���������Ժͽӿ�
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
	/********************���¶���CFileTransferClientDlg���ڲ�����***************/
	/***************************************************************************/
	void Init();													//��ʼ����ͼ������Ϣ
	HICON m_hIcon;
	/***************������CFileTransferClientDlg����Ϣӳ��**********************/
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
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);		//������ȸ��µ���Ϣӳ��
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);		//��С����������Ϣӳ��
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	/***************************************************************************/
	/******************������CFileTransferClientDlg�Ļ�������*******************/
	/***************************************************************************/
	CModel * _model;												//����ģ�Ͷ���
	CControl * _control;											//��ͼ���ƶ���
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANSFERCLIENTDLG_H__7F3FE5F8_5185_430F_AC76_C2562D1105F4__INCLUDED_)
