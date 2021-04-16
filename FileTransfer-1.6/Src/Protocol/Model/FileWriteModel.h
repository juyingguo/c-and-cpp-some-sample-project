/*******************************版权信息*************************************
 * 文件名称 : FileWriteModel.h
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.08
 * 文件说明 : interface for the CFileWriteModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEWRITEMODEL_H__70BE29EF_B7E3_42B5_9665_046321D806AD__INCLUDED_)
#define AFX_FILEWRITEMODEL_H__70BE29EF_B7E3_42B5_9665_046321D806AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileModel.h"

/*
 * 类的名称 : CFileWriteModel
 * 基本用途 : 实现抽象文件模型的接口，提供写文件的基本方法
 */

class CFileWriteModel : public CFileModel  
{
public:
	CFileWriteModel(CString _strFilePath, Size _fileSize);
	virtual ~CFileWriteModel();

protected:
	/***************************************************************************/
	/********************以下是CFileWriteModel的内部方法************************/
	/***************************************************************************/
	void Copy(void *lpFile, void *_buf, DWORD _size);
};

#endif // !defined(AFX_FILEWRITEMODEL_H__70BE29EF_B7E3_42B5_9665_046321D806AD__INCLUDED_)
