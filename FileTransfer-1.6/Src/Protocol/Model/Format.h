/*******************************版权信息*************************************
 * 文件名称 : Format.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.04
 * 文件说明 : interface for the CFormat class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMAT_H__A6495811_7640_491C_957C_43E10B79EDC7__INCLUDED_)
#define AFX_FORMAT_H__A6495811_7640_491C_957C_43E10B79EDC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

/*
 * 类的名称 : CFormat
 * 基本用途 : 定义格式化相关数据信息为字符串形式的接口
 */

class CFormat  
{
public:
	
	/***************************************************************************/
	/********************以下是CFormat对外提供的接口****************************/
	/***************************************************************************/
	static CString ToString(Size _remainedTime);				//获取剩余时间的CString形式
	static CString ToString(Size _speed, long _ratio);			//格式化速度信息为CString形式
	static CString ToString(Size _size, int _radix);			//格式化Size类型数据为CString形式
	static CString FormatFileSize(Size _fileSize);				//格式化文件大小为指定格式的CString形式
	CFormat();
	virtual ~CFormat();
private:
	static CString Format(Size _time);								//格式化时间信息为00 - 60之间的字符串信息
};

#endif // !defined(AFX_FORMAT_H__A6495811_7640_491C_957C_43E10B79EDC7__INCLUDED_)
