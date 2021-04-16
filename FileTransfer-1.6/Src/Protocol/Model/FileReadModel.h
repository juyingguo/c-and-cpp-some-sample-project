/*******************************版权信息*************************************
 * 文件名称 : FileReadModel.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.08
 * 文件说明 : interface for the CFileReadModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEREADMODEL_H__C3C6F498_AABE_447D_A913_355A64E047DC__INCLUDED_)
#define AFX_FILEREADMODEL_H__C3C6F498_AABE_447D_A913_355A64E047DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileModel.h"

/*
 * 类的名称 : CFileReadModel
 * 基本用途 : 实现抽象文件模型的接口，提供读文件的基本方法
 */

class CFileReadModel : public CFileModel  
{
public:
	CFileReadModel(CString _strFilePath, Size _fileSize);
	virtual ~CFileReadModel();

protected:
	/***************************************************************************/
	/*********************以下是CFileReadModel的内部方法************************/
	/***************************************************************************/
	void Copy(void *lpFile, void *_buf, DWORD _size);
};

#endif // !defined(AFX_FILEREADMODEL_H__C3C6F498_AABE_447D_A913_355A64E047DC__INCLUDED_)
