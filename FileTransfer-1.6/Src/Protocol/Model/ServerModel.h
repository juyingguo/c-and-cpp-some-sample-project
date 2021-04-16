/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ServerModel.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : interface for the CServerModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERMODEL_H__4DBF5C17_82F9_4E5A_B91D_97615CA2FD49__INCLUDED_)
#define AFX_SERVERMODEL_H__4DBF5C17_82F9_4E5A_B91D_97615CA2FD49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Model.h"

/*
 * ������� : CServerModel
 * ������; : ʵ�ֻ�������ģ�͵Ľӿڣ��ṩserverģ�͵Ļ�������
 */

class CServerModel : public CModel  
{
public:
	/***************************************************************************/
	/*****************������CServerModel�����ṩ�Ľӿ�**************************/
	/***************************************************************************/

	/********************CServerModel�Ĺ�������*********************************/
	void AcceptProcess();								//�������ӵĴ�������
	int TransferFileInfo();								//�����ļ���Ϣ
	void TransferProcess();								//�����ļ��Ĺ���
	
	CServerModel(HWND m_hWnd);
	virtual ~CServerModel();
protected:
	/***************************************************************************/
	/*****************������CServerModel���ڲ�����******************************/
	/***************************************************************************/
	void InitSock();									//��ʼ���׽���
	void InitMaxConnect(CConfigFile _file);				//��ʼ�����������
	void Handle();										//��Ϣ��������
	void SetMessageType(Message &_msg, MessageType _messgaeType);
														//������Ϣ������
private:
	/***************************************************************************/
	/*****************���¶���CServerModel�Ļ�������****************************/
	/***************************************************************************/
	SOCKET _localSock;									//�����׽���
	int _maxConnect;									//���������
};

#endif // !defined(AFX_SERVERMODEL_H__4DBF5C17_82F9_4E5A_B91D_97615CA2FD49__INCLUDED_)