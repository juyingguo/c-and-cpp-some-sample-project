/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileReadModel.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.08
 * �ļ�˵�� : interface for the CFileReadModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEREADMODEL_H__C3C6F498_AABE_447D_A913_355A64E047DC__INCLUDED_)
#define AFX_FILEREADMODEL_H__C3C6F498_AABE_447D_A913_355A64E047DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileModel.h"

/*
 * ������� : CFileReadModel
 * ������; : ʵ�ֳ����ļ�ģ�͵Ľӿڣ��ṩ���ļ��Ļ�������
 */

class CFileReadModel : public CFileModel  
{
public:
	CFileReadModel(CString _strFilePath, Size _fileSize);
	virtual ~CFileReadModel();

protected:
	/***************************************************************************/
	/*********************������CFileReadModel���ڲ�����************************/
	/***************************************************************************/
	void Copy(void *lpFile, void *_buf, DWORD _size);
};

#endif // !defined(AFX_FILEREADMODEL_H__C3C6F498_AABE_447D_A913_355A64E047DC__INCLUDED_)
