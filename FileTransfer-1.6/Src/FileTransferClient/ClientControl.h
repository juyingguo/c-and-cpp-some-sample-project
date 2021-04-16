/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ClientControl.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * ����ԭ�� : ������Size�ṹ��������������
 * �ļ�˵�� : interface for the CClientControl class.
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
 * ������� : CClientControl
 * ������; : ʵ�ֳ����CControl�ӿڣ��ṩClient�˵���ͼ��Ϣ�Ŀ��Ʒ���
 */

class CClientControl : public CControl  
{
public:
	/***************************************************************************/
	/*******************������CClientControl�����ṩ�Ľӿ�**********************/
	/***************************************************************************/
	void Browzer();												//�����ļ�·��
	int IsValidPath();											//�ж�·���Ƿ�Ϸ�
	void ConnectProcess(CString _strRemoteAddr);				//����TCP���ӵĴ������
	void BeginTransfer();										//�����ļ���������̿���
	void MessageProcess(UINT message);							//��ͼ��Ϣ�Ĵ���
	CClientControl(int _uID, CFileTransferClientDlg * _pView, CModel * _model);
	virtual ~CClientControl();
protected:
	/***************************************************************************/
	/*******************������CClientControl���ڲ�����**************************/
	/***************************************************************************/
	void InitControlID();										//��ʼ���ؼ�ID
	void InitControlStateMap();									//��ʼ���ؼ�״̬ӳ���
	void InitFilePath();										//��ʼ����ͼ���ļ�·����Ϣ
	void ResetView();											//������ͼ��Ϣ
	
	void SetFileInfo();											//�����ļ���Ϣ
	void SetFileSize();											//������ͼ���ļ���С��Ϣ
private:
	/***************************************************************************/
	/*******************������CClientControl������******************************/
	/***************************************************************************/
	CFileTransferClientDlg * _pView;							//��ͼ����ָ��
};

#endif // !defined(AFX_CLIENTCONTROL_H__9FF30A33_D6A7_40A1_8D05_14B06567EFD1__INCLUDED_)
