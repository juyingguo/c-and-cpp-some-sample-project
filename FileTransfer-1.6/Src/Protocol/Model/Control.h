/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : Control.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * ����ԭ�� : ������������ΪSize
 * �ļ�˵�� : interface for the CControl class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROL_H__855FEBE6_64F6_4F3E_B972_5AE55046BA3D__INCLUDED_)
#define AFX_CONTROL_H__855FEBE6_64F6_4F3E_B972_5AE55046BA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

class CModel;

/*
 * ������� : CControl
 * ������; : �ṩ��ͼ��Ϣ�Ŀ��Ʒ���
 */

class CControl  
{
public:
	
	/***************************************************************************/
	/*********************������CControl�����ṩ�Ľӿ�**************************/
	/***************************************************************************/
	int JudgeMessage(UINT message);							//�ж���Ϣ����
	void LoadHelp();										//��������ļ�
	int GetProgress();										//��ȡ�������
	void UpdateControl(CDialog * _pView);					//���´���ʱ�Ŀؼ���Ϣ
	void ToTray();											//��С��������
	void DeleteTray();										//ɾ������
	CString SizeToString(Size _size, int _radix);			//�ṩת��SizeΪCString�Ľӿ�
	/*********************���·����ӳٵ�������ʵ��******************************/
	virtual void Browzer();									//���ô�������ļ�·��
	virtual int IsValidPath();								//�ж�·���Ƿ�Ϸ�
	virtual void ConnectProcess(CString _strRemoteAddr);	//����TCP���ӵĴ���client��ʵ��
	virtual void BeginTransfer();							//�����ļ���������̿���
	virtual void MessageProcess(UINT message);				//��ͼ��Ϣ�Ĵ������
	
	CControl();
	virtual ~CControl();
protected:
	/***************************************************************************/
	/*********************������CControl���ڲ�����******************************/
	/***************************************************************************/
	void Init();											//��ʼ����ͼ״̬
	void InitStateMap();									//��ʼ��״̬�ַ���Ϣӳ���
	void ResetControl(CDialog * _pView);					//���ÿؼ�״̬
	void UpdateView();										//������ͼ��Ϣ
	CString GetSpeed();										//��ȡʵʱ�����ٶ���Ϣ
	CString GetRemainedTime();								//��ȡʣ��ʱ��
	/********************���·����ӳٵ�����ʵ��*********************************/
	virtual void InitControlID();							//��ʼ���ؼ���Դ��
	virtual void InitControlStateMap();						//��ʼ���ؼ�״̬ӳ���
	virtual void InitFilePath();							//�����ļ�·����client��ʵ��
	virtual void ResetView();								//������ͼ��Ϣ
	virtual void SetFileInfo();								//����ģ�͵��ļ���Ϣ
protected:
	/***************************************************************************/
	/*******************������CControl�����Զ���********************************/
	/***************************************************************************/
	CModel * _model;										//ģ�Ͷ���ָ��
	HWND m_hWnd;											//��ͼ���
	int _uID;												//��ͼ��Դ��

	int _stateID;											//״̬�ؼ�����Դ�ţ�����ʵ����
	CString _strStateMap[STATE_NUM];						//״̬�ַ���ӳ�������ʵ����
	
	int _controlID[MAX_CONTROL_NUM];						//�ؼ���Դ�ż��ϣ�����ʵ����
	int _controlNum;										//��Ҫ�ı�״̬�Ŀؼ�����������ʵ����
	int _ctlStateMap[MAX_CONTROL_NUM][STATE_NUM];			//�ؼ�״̬ӳ�������ʵ����

	int _speedID;											//�ٶȿؼ�����Դ�ţ�����ʵ����
	int _remainedTimeID;									//ʣ��ʱ��ؼ���Ӧ����Դ�ţ�����ʵ����
	//////////////////////////2010.03.04���£�������������///////////////////////
	Size _startOffset;										//��ʼλ�ã������ٶȸ���
	Size _endOffset;										//��ֹλ�ã������ٶȸ���
	/////////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_CONTROL_H__855FEBE6_64F6_4F3E_B972_5AE55046BA3D__INCLUDED_)
