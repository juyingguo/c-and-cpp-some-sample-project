/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileWriteModel.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.08
 * �ļ�˵�� : interface for the CFileWriteModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEWRITEMODEL_H__70BE29EF_B7E3_42B5_9665_046321D806AD__INCLUDED_)
#define AFX_FILEWRITEMODEL_H__70BE29EF_B7E3_42B5_9665_046321D806AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileModel.h"

/*
 * ������� : CFileWriteModel
 * ������; : ʵ�ֳ����ļ�ģ�͵Ľӿڣ��ṩд�ļ��Ļ�������
 */

class CFileWriteModel : public CFileModel  
{
public:
	CFileWriteModel(CString _strFilePath, Size _fileSize);
	virtual ~CFileWriteModel();

protected:
	/***************************************************************************/
	/********************������CFileWriteModel���ڲ�����************************/
	/***************************************************************************/
	void Copy(void *lpFile, void *_buf, DWORD _size);
};

#endif // !defined(AFX_FILEWRITEMODEL_H__70BE29EF_B7E3_42B5_9665_046321D806AD__INCLUDED_)
