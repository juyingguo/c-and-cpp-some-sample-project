/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ClientModel.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.07
 * �ļ�˵�� : interface for the CClientModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTMODEL_H__D5F3703B_E221_4BF4_B5FA_CA03956161F9__INCLUDED_)
#define AFX_CLIENTMODEL_H__D5F3703B_E221_4BF4_B5FA_CA03956161F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Model.h"

/*
 * ������� : CClientModel
 * ������; : ʵ�ֻ�������ģ�͵Ľӿڣ��ṩclientģ�͵Ļ�������
 */

class CClientModel : public CModel  
{
public:
	/***************************************************************************/
	/****************���¶���CClientModel�����ṩ�ķ���*************************/
	/***************************************************************************/
	void ConnectProcess();											//����TCP����
	int TransferFileInfo();											//�ļ���Ϣ�Ĵ���
	void TransferProcess();											//�ļ��������
	
	CClientModel(HWND m_hWnd);
	virtual ~CClientModel();
protected:
	/***************************************************************************/
	/****************���¶���CClientModel���ڲ�����*****************************/
	/***************************************************************************/
	void InitRemoteAddr(CConfigFile _file);							//��ʼ��server�˵�ַ
	void InitSock();												//��ʼ��sock��Ϣ
	void Handle();													//��Ϣ�������
	void SetMessageType(Message &_msg, MessageType _messgaeType);	//������Ϣ����
};

#endif // !defined(AFX_CLIENTMODEL_H__D5F3703B_E221_4BF4_B5FA_CA03956161F9__INCLUDED_)
