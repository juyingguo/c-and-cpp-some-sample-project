/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ValidJudge.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.24
 * �ļ�˵�� : implementation of the CValidJudge class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidJudge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/***************************************************************************/
/***************������CValidJudge�Ĺ��췽��*********************************/
/***************************************************************************/

CValidJudge::CValidJudge()
{
	_min = INVALID_VALUE;
	_max = INVALID_VALUE;
}

CValidJudge::CValidJudge(CString _strConfig)
{
	this->_strConfig = _strConfig;
	_min = INVALID_VALUE;
	_max = INVALID_VALUE;
}

CValidJudge::CValidJudge(CString _strConfig, int _max, int _min)
{
	this->_strConfig = _strConfig;
	this->_max = _max;
	this->_min = _min;
}

CValidJudge::~CValidJudge()
{

}
/***************************************************************************/
/**********************������CValidJudge�����ṩ�ķ���**********************/
/***************************************************************************/

/*****************CValidJudge���Ե�Set����**********************************/

void CValidJudge::SetConfig(CString _strConfig)
{
	this->_strConfig = _strConfig;
}

void CValidJudge::SetRange(int _max, int _min)
{
	this->_max = _max;
	this->_min = _min;
}
/*---------------------------------------------------------------------------
 * �������� : Judge
 * �������� : ��������������Ϣ�ĺϷ����ж�
 * ��ڲ��� : ��
 * ���ڲ��� : �Ϸ�����TRUE, ���򷵻�FALSE
 ---------------------------------------------------------------------------*/
int CValidJudge::Judge()
{
	//�ж��������ǲ��ǺϷ�������
	for (int i = 0; i < _strConfig.GetLength(); i++)
	{
		char _cTemp = _strConfig.GetAt(i);
		int _nAscii = (int)(_cTemp);
		if (_nAscii < 0x30 || _nAscii > 0x39)		//���ֵ�ASCII�뷶Χ
		{
			return FALSE;
		}
	}
	//�ж�������ķ�Χ�Ƿ�Ϸ�
	int _nTemp = atoi(_strConfig);
	if (_nTemp < _min || _nTemp > _max)
	{
		return FALSE;
	}
	return TRUE;
}
