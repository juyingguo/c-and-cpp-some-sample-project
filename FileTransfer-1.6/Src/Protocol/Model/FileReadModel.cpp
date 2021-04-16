/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileReadModel.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.08
 * �ļ�˵�� : implementation of the CFileReadModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileReadModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 * �������� : CFileReadModel
 * �������� : ʵ���������ļ�ģ�͵Ļ�������
 * ��ڲ��� : CString _strFilePath, �ļ�·��; Size _fileSize, �ļ���С
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
CFileReadModel::CFileReadModel(CString _strFilePath, Size _fileSize)
{
	this->_strFilePath		= _strFilePath;
	this->_fileSize			= _fileSize;
	dwDesiredAccess			= GENERIC_READ;
	dwShareMode				= FILE_SHARE_READ;
	dwCreationDisposition	= OPEN_EXISTING;
	dwErrorReturnValue		= READ_ERROR;
	flProtect				= PAGE_READONLY;
	dwAccess				= FILE_MAP_READ;
}

CFileReadModel::~CFileReadModel()
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
void CFileReadModel::Copy(void *lpFile, void *_buf, DWORD _size)
{
	memcpy(_buf, lpFile, _size);
}
