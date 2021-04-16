/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : Model.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * �������� : ������������ΪSize
 * �ļ�˵�� : interface for the CModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL_H__06C4436C_1F1D_424E_B543_0DC8B6AA2CFA__INCLUDED_)
#define AFX_MODEL_H__06C4436C_1F1D_424E_B543_0DC8B6AA2CFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"
#include "..\KERNEL\AppConfig.h"	// Added by ClassView

UINT ConnectProcessThread(LPVOID lpParam);				//����TCP�����̵߳Ĵ���
UINT AcceptProcessThread(LPVOID lpParam);				//�������ӵ��̷߳�������
UINT MessageProcessThread(LPVOID lpParam);				//ȫ�ֵ���Ϣ�̴߳�����������
UINT TransferProcessThread(LPVOID lpParam);				//ȫ�ֵ��ļ������̴߳�����������

/*
 * ������� : CModel
 * ������; : �����ļ�����Ļ���ģ��
 */

class CConfigFile;

class CModel  
{
public:
	
	
	/***************************************************************************/
	/************************���¶���CModel�����ṩ�Ľӿ�***********************/
	/***************************************************************************/

	/************************CModel���Ե�get��set����***************************/
	int GetState();

	CString GetAddress();

	void SetRemoteAddr(CString _strRemoteAddr);
	CString GetRemoteAddr();

	void SetFilePath(CString _strFilePath);

	void SetFileInfo(FileInfo _fileInfo);
	FileInfo GetFileInfo();

	void SetOffset(Size _offset);
	Size GetOffset();

	/************************CModel�Ĺ�������**********************************/
	void BeginConnectThread();								//����TCP�����߳�
	void BeginAcceptThread();								//��������TCP�����߳�
	void BeginMessageThread();								//������Ϣ�����߳�
	void BeginTransferThread();								//���������߳�
	void MessageProcess();									//��Ϣ�������
	void InterruptTransfer();								//�жϴ���
	void CloseConnect();									//�ر�����
	/************************�ӳٵ�����ʵ�ֵķ���******************************/
	
	virtual void ConnectProcess();							//���Ӵ���clientʵ��
	virtual void AcceptProcess();							//�������Ӵ���serverʵ��
	virtual int TransferFileInfo();							//�����ļ���Ϣ
	virtual void TransferProcess();							//�ļ��������
	
	CModel();
	virtual ~CModel();
protected:
	/***************************************************************************/
	/*****************************CModel���ڲ�����******************************/
	/***************************************************************************/
	void Init(HWND m_hWnd);									//�����ʼ������
	void InitPort(CConfigFile _file);						//��ʼ��server�˿�
	int InitSocket(SOCKET & _sock);							//��ʼ��socket
	CString GetHostIPAddress();								//��ȡ���ص�ַ
	void Reset();											//����ģ�͵�����
	int Send(Message & _msg);								//������Ϣ
	void TransferOver();									//��������ľ��崦��
	void CloseConnectProcess();								//�ر����ӵľ��崦��
	/************************�������ӳٵ�������ʵ�ֵķ���***********************/
	virtual void InitMaxConnect(CConfigFile _file);			//��ʼ�������������server��ʵ��
	virtual void InitRemoteAddr(CConfigFile _file);			//��ʼ��Զ�̵�ַ��client��ʵ��
	virtual void InitSock();								//��ʼ���׽��ֵ������������
	virtual void Handle();									//ʵ�ʵ���Ϣ�������
	virtual void SetMessageType(Message &_msg, MessageType _messgaeType);
															//������Ϣ�ṹ����Ϣ���Ͳ���
protected:
	
	/***************************************************************************/
	/*************************���¶���CModel�Ļ�������**************************/
	/***************************************************************************/
	HWND m_hWnd;											//��ͼ����������ģ�ͺ���ͼ��ͨ��
	int _state;												//ģ��״̬�ı�ʶ
	SOCKET _connectedSock;									//����TCP���ӵ�SOCKET
	CString _strAddress;									//���ص�ַ
	CString _strRemoteAddr;									//Զ�ص�ַ
	int _port;												//server����Ϣ�����˿�
	Message _buffer;										//������Ϣ�Ļ�����
	
	CString _strFilePath;									//��������ļ�·��
	FileInfo _fileInfo;										//��������ļ���Ϣ
	//long _offset;											//�Ѵ�����ļ����������ڶϵ�����
	//DWORD _offSet;										//�Ѵ�����ļ���С�����ڶϵ�����
	////////////2010.03.04���£�������������ΪSize///////////////////////////////
	Size _offset;											//�Ѵ�����ļ���С�����ڶϵ�����
	/////////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_MODEL_H__06C4436C_1F1D_424E_B543_0DC8B6AA2CFA__INCLUDED_)
