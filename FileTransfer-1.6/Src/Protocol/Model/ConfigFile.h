/*******************************版权信息*************************************
 * 文件名称 : ConfigFile.h
 * 文件版本 : 2.1
 * 开发人员 : Yao
 * 更新日期 : 2010.02.24
 * 更新原因 : 修正连续载入配置文件失败的Bug
			  增加配置项的合法性逻辑判断
 * 更新日期 : 2010.03.08
 * 更新原因 : 提供配置项合法性判断的API
 * 文件说明 : interface for the CConfigFile class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGFILE_H__228CD53D_B771_41C9_A61F_A39C9E691BCA__INCLUDED_)
#define AFX_CONFIGFILE_H__228CD53D_B771_41C9_A61F_A39C9E691BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"
/*
 * 类的名称 : CConfigFile
 * 基本用途 : 定义配置文件的基本操作，供上层的组件调用
 */

class CConfigFile  
{
public:
	
	/***************************************************************************/
	/***************以下定义配置文件类对外界提供的访问接口**********************/
	/***************************************************************************/

	/*****************以下定义配置文件类的属性的set和get方法********************/
	CString GetValue(int _index);
	CString GetItem(int _index);
	int GetLength();

	static int Judge(CString _strConfig, int _max, int _min);
														//配置项目合法性判断
	int Load();											//载入配置文件
	CConfigFile();
	virtual ~CConfigFile();
protected:
	/***************************************************************************/
	/*****************以下定义配置文件类的内部方法******************************/
	/***************************************************************************/
	void InitFilePath();								//初始化文件基本路径
	CString GetItem(CString _strLoad);					//根据读取的配置项信息获取配置项的名称
	CString GetValue(CString _strLoad);					//根据读取的配置项信息获取配置项的值
	int Judge(CString _strLoad);						//配置项首项合法性判断
protected:
	/***************************************************************************/
	/******************以下定义配置文件类的基本的属性***************************/
	/***************************************************************************/
	CString _strFilePath;								//文件的基本路径
	CString _strItem[MAX_CONFIG_ITEM];					//配置项的名称的缓存
	CString _strValue[MAX_CONFIG_ITEM];					//配置项的值的缓存
	int _length;										//配置文件的长度
};

#endif // !defined(AFX_CONFIGFILE_H__228CD53D_B771_41C9_A61F_A39C9E691BCA__INCLUDED_)
