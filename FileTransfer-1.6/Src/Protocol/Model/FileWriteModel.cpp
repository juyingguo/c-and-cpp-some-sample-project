/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileWriteModel.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.08
 * �ļ�˵�� : implementation of the CFileWriteModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileWriteModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 * �������� : CFileWriteModel
 * �������� : ʵ���������ļ�ģ�͵Ļ�������
 * ��ڲ��� : CString _strFilePath, �ļ�·��; Size _fileSize, �ļ���С
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
CFileWriteModel::CFileWriteModel(CString _strFilePath, Size _fileSize)
{
	this->_strFilePath		= _strFilePath;
	this->_fileSize			= _fileSize;
	dwDesiredAccess			= GENERIC_WRITE|GENERIC_READ;
	dwShareMode				= FILE_SHARE_WRITE;
	dwCreationDisposition	= OPEN_ALWAYS;
	dwErrorReturnValue		= WRITE_ERROR;
	flProtect				= PAGE_READWRITE;
	dwAccess				= FILE_MAP_WRITE;
}

CFileWriteModel::~CFileWriteModel()
{

}
/*---------------------------------------------------------------------------
 * �������� : Copy
 * �������� : ʵ�ֶ��ļ��Ŀ�������
 * ��ڲ��� : void *lpFile, ָ���ļ���ָ��
			  void *_buf, ָ�򻺳�����ָ��
			  DWORD _size, ��Ҫ�����Ĵ�С
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CFileWriteModel::Copy(void *lpFile, void *_buf, DWORD _size)
{
	memcpy(lpFile, _buf, _size);
}
