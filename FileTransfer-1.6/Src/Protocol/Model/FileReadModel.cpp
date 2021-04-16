/*******************************版权信息*************************************
 * 文件名称 : FileReadModel.cpp
 * 文件版本 : 1.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.08
 * 文件说明 : implementation of the CFileReadModel class.
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
 * 函数名称 : CFileReadModel
 * 函数功能 : 实例化抽象文件模型的基本参数
 * 入口参数 : CString _strFilePath, 文件路径; Size _fileSize, 文件大小
 * 出口参数 : 无
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
 * 函数名称 : Copy
 * 函数功能 : 实现读文件的拷贝过程
 * 入口参数 : void *lpFile, 指向文件的指针
			  void *_buf, 指向缓冲区的指针
			  DWORD _size, 需要拷贝的大小
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CFileReadModel::Copy(void *lpFile, void *_buf, DWORD _size)
{
	memcpy(_buf, lpFile, _size);
}
