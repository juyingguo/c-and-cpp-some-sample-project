/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ValidJudge.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.24
 * �ļ�˵�� : interface for the CValidJudge class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALIDJUDGE_H__3E2008EA_E23B_45D9_8D3D_DCCB7CDB5D9A__INCLUDED_)
#define AFX_VALIDJUDGE_H__3E2008EA_E23B_45D9_8D3D_DCCB7CDB5D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

/*
 * ������� : CValidJudge
 * ������; : �ṩ����������Ϣ�Ϸ����жϵ�ͳһ�ӿ�
 */

class CValidJudge  
{
public:
	/***************************************************************************/
	/*****************CValidJudge�����ṩ�Ľӿ�*********************************/
	/***************************************************************************/

	/*****************CValidJudge���Ե�Set����**********************************/
	void SetConfig(CString _strConfig);
	void SetRange(int _max, int _min);							//���������Χ

	int Judge();												//�Ϸ����ж�
	
	CValidJudge(CString _strConfig, int _max, int _min);
	CValidJudge(CString _strConfig);
	CValidJudge();
	virtual ~CValidJudge();

protected:
	/***************************************************************************/
	/*********************������CValidJudge������*******************************/
	/***************************************************************************/
	CString _strConfig;											//����������
	int _max;													//���Χ����
	int _min;													//��С��Χ����
};

#endif // !defined(AFX_VALIDJUDGE_H__3E2008EA_E23B_45D9_8D3D_DCCB7CDB5D9A__INCLUDED_)
