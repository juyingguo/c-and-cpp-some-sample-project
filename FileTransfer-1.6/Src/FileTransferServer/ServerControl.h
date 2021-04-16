/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ServerControl.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : interface for the CServerControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERCONTROL_H__D7587300_1AE0_471D_86E7_57114C624834__INCLUDED_)
#define AFX_SERVERCONTROL_H__D7587300_1AE0_471D_86E7_57114C624834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileTransferServer.h"

class CFileTransferServerDlg;

/*
 * ������� : CServerControl
 * ������; : ʵ�ֳ����CControl�ӿڣ��ṩServer�˵���ͼ��Ϣ�Ŀ��Ʒ���
 */

class CServerControl : public CControl  
{
public:
	/***************************************************************************/
	/*********************������CServerControl�����ṩ�Ľӿ�********************/
	/***************************************************************************/
	void Browzer();										//�����ļ�·��
	int IsValidPath();									//�ж�·���ĺϷ���
	void BeginTransfer();								//�����ļ�����
	void MessageProcess(UINT message);					//��ͼ��Ϣ�������
	
	CServerControl(int _uID, CFileTransferServerDlg * _pView, CModel * _model);
	virtual ~CServerControl();
protected:
	/***************************************************************************/
	/*********************������CServerControl���ڲ�����************************/
	/***************************************************************************/
	void InitControlID();								//��ʼ���ؼ�ID
	void InitControlStateMap();							//��ʼ���ؼ�״̬ӳ���
	
	void ResetView();									//������ͼ��Ϣ
	void SetFileInfo();									//�����ļ���Ϣ
private:
	/***************************************************************************/
	/********************������CServerControl�����Զ���*************************/
	/***************************************************************************/
	CFileTransferServerDlg * _pView;					//��ͼ����ָ��
};

#endif // !defined(AFX_SERVERCONTROL_H__D7587300_1AE0_471D_86E7_57114C624834__INCLUDED_)
