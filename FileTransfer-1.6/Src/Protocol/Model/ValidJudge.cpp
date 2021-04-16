/*******************************版权信息*************************************
 * 文件名称 : ValidJudge.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.24
 * 文件说明 : implementation of the CValidJudge class.
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
/***************以下是CValidJudge的构造方法*********************************/
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
/**********************以下是CValidJudge对外提供的方法**********************/
/***************************************************************************/

/*****************CValidJudge属性的Set方法**********************************/

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
 * 函数名称 : Judge
 * 函数功能 : 进行数字配置信息的合法性判断
 * 入口参数 : 无
 * 出口参数 : 合法返回TRUE, 否则返回FALSE
 ---------------------------------------------------------------------------*/
int CValidJudge::Judge()
{
	//判断配置项是不是合法的数字
	for (int i = 0; i < _strConfig.GetLength(); i++)
	{
		char _cTemp = _strConfig.GetAt(i);
		int _nAscii = (int)(_cTemp);
		if (_nAscii < 0x30 || _nAscii > 0x39)		//数字的ASCII码范围
		{
			return FALSE;
		}
	}
	//判断配置项的范围是否合法
	int _nTemp = atoi(_strConfig);
	if (_nTemp < _min || _nTemp > _max)
	{
		return FALSE;
	}
	return TRUE;
}
