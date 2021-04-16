/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : Format.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * �ļ�˵�� : interface for the CFormat class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMAT_H__A6495811_7640_491C_957C_43E10B79EDC7__INCLUDED_)
#define AFX_FORMAT_H__A6495811_7640_491C_957C_43E10B79EDC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

/*
 * ������� : CFormat
 * ������; : �����ʽ�����������ϢΪ�ַ�����ʽ�Ľӿ�
 */

class CFormat  
{
public:
	
	/***************************************************************************/
	/********************������CFormat�����ṩ�Ľӿ�****************************/
	/***************************************************************************/
	static CString ToString(Size _remainedTime);				//��ȡʣ��ʱ���CString��ʽ
	static CString ToString(Size _speed, long _ratio);			//��ʽ���ٶ���ϢΪCString��ʽ
	static CString ToString(Size _size, int _radix);			//��ʽ��Size��������ΪCString��ʽ
	static CString FormatFileSize(Size _fileSize);				//��ʽ���ļ���СΪָ����ʽ��CString��ʽ
	CFormat();
	virtual ~CFormat();
private:
	static CString Format(Size _time);								//��ʽ��ʱ����ϢΪ00 - 60֮����ַ�����Ϣ
};

#endif // !defined(AFX_FORMAT_H__A6495811_7640_491C_957C_43E10B79EDC7__INCLUDED_)
