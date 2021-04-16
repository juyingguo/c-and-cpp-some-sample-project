/*******************************版权信息*************************************
 * 文件名称 : ValidJudge.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.02.24
 * 文件说明 : interface for the CValidJudge class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALIDJUDGE_H__3E2008EA_E23B_45D9_8D3D_DCCB7CDB5D9A__INCLUDED_)
#define AFX_VALIDJUDGE_H__3E2008EA_E23B_45D9_8D3D_DCCB7CDB5D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

/*
 * 类的名称 : CValidJudge
 * 基本用途 : 提供数字配置信息合法性判断的统一接口
 */

class CValidJudge  
{
public:
	/***************************************************************************/
	/*****************CValidJudge对外提供的接口*********************************/
	/***************************************************************************/

	/*****************CValidJudge属性的Set方法**********************************/
	void SetConfig(CString _strConfig);
	void SetRange(int _max, int _min);							//设置配置项范围

	int Judge();												//合法性判断
	
	CValidJudge(CString _strConfig, int _max, int _min);
	CValidJudge(CString _strConfig);
	CValidJudge();
	virtual ~CValidJudge();

protected:
	/***************************************************************************/
	/*********************以下是CValidJudge的属性*******************************/
	/***************************************************************************/
	CString _strConfig;											//配置项内容
	int _max;													//最大范围定义
	int _min;													//最小范围定义
};

#endif // !defined(AFX_VALIDJUDGE_H__3E2008EA_E23B_45D9_8D3D_DCCB7CDB5D9A__INCLUDED_)
