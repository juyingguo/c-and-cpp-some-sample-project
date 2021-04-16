/*******************************版权信息*************************************
 * 文件名称 : FileModel.cpp
 * 文件版本 : 2.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.08
 * 更新原因 : 调整文件操作的模型为内存文件映射的方式
 * 文件说明 : implementation of the CFileModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileModel::CFileModel()
{

}
CFileModel::~CFileModel()
{

}

/***************************************************************************/
/********************以下是CFileModel对外提供的接口*************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : GetSize
 * 函数功能 : 获取文件大小
 * 入口参数 : 无
 * 出口参数 : 文件大小
 ---------------------------------------------------------------------------*/
Size CFileModel::GetSize(CString _strFilePath)
{
	HANDLE hFile = CreateFile(_strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return INVALID_VALUE;
	}
	DWORD dwLowSize, dwHighSize;
	dwLowSize = GetFileSize(hFile, &dwHighSize);
	LARGE_INTEGER _li;
	_li.LowPart = dwLowSize;
	_li.HighPart = dwHighSize;
	
	CloseHandle(hFile);
	return (Size)(_li.QuadPart);
}

/*---------------------------------------------------------------------------
 * 函数名称 : Read
 * 函数功能 : 从指定路径文件读取指定偏移处的指定大小的文件块
 * 入口参数 : CString _strFilePath, 文件路径;
			  Size _fileSize, 文件大小
			  void *_buf, 文件读取缓冲区
			  Size _offset, 读取的偏移处
			  DWORD _size, 指定的读取的内容大小
 * 出口参数 : 正确返回实际读取的长度, 否则返回READ_ERROR
 ---------------------------------------------------------------------------*/
DWORD CFileModel::Read(CString _strFilePath, Size _fileSize, void *_buf, Size _offset, DWORD _size)
{
	HANDLE hFile = CreateFile(_strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFile Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		return READ_ERROR;
	}
	
	LARGE_INTEGER _li;
	_li.QuadPart = _fileSize;
	
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, (DWORD)(_li.HighPart), (DWORD)(_li.LowPart), NULL);
	if (hFileMap == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFileMapping Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		return READ_ERROR;
	}
	CloseHandle(hFile);
	
	_li.QuadPart = _offset;
	LPBYTE lpFile = (LPBYTE)MapViewOfFile(hFileMap, FILE_MAP_READ, (DWORD)(_li.HighPart), (DWORD)(_li.LowPart), _size);//_offset >> 32  _offset & 0xFFFFFFFF
	if (lpFile == NULL)
	{
		TRACE("MapViewOfFile Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		CloseHandle(hFileMap);
		return READ_ERROR;
	}
	
	memcpy(_buf, lpFile, _size);
	UnmapViewOfFile(lpFile);
	CloseHandle(hFileMap);
	return _size;
}

/*---------------------------------------------------------------------------
 * 函数名称 : Write
 * 函数功能 : 将缓冲区内容写入指定路径的文件的指定偏移位置处
 * 入口参数 : CString _strFilePath, 文件路径;
			  Size _fileSize, 文件大小
			  void *_buf, 需要保存内容的缓冲区
			  Size _offset, 写入的偏移处
			  DWORD _size, 指定的写入的内容大小
 * 出口参数 : 正确返回实际写入的内容的大小; 否则返回FALSE
 ---------------------------------------------------------------------------*/
DWORD CFileModel::Write(CString _strFilePath, Size _fileSize, const void *_buf, Size _offset, DWORD _size)
{
	HANDLE hFile = CreateFile(_strFilePath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFile Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		return WRITE_ERROR;
	}
	
	LARGE_INTEGER _li;
	_li.QuadPart = _fileSize;
	
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, (DWORD)(_li.HighPart), (DWORD)(_li.LowPart), NULL);
	if (hFileMap == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFileMapping Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		return WRITE_ERROR;
	}
	CloseHandle(hFile);
	
	_li.QuadPart = _offset;
	LPBYTE lpFile = (LPBYTE)MapViewOfFile(hFileMap, FILE_MAP_WRITE, (DWORD)(_li.HighPart), (DWORD)(_li.LowPart), _size);
	if (lpFile == NULL)
	{
		TRACE("MapViewOfFile Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		CloseHandle(hFileMap);
		return WRITE_ERROR;
	}
	
	memcpy(lpFile, _buf, _size);
	UnmapViewOfFile(lpFile);
	CloseHandle(hFileMap);
	return _size;
}

/*---------------------------------------------------------------------------
 * 函数名称 : Serialize
 * 函数功能 : 对指定的缓冲区进行序列化操作
 * 入口参数 : void * _buf, 待操作文件的缓存区, 提高模型工作的效率;
			  Size _offset, 操作的偏移起点;
			  DWORD _size, 缓冲区的大小
 * 出口参数 : 出错返回READ_ERROR, 正确返回实际操作的长度_size
 * 更新日期 : 2010.03.08
 * 更新原因 : 操作文件的方式调整为内存文件映射
 ---------------------------------------------------------------------------*/
DWORD CFileModel::Serialize(void *_buf, Size _offset, DWORD _size)
{
	HANDLE hFile = CreateFile(_strFilePath, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFile Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		return dwErrorReturnValue;
	}
	
	LARGE_INTEGER _li;
	_li.QuadPart = _fileSize;
	
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, flProtect, (DWORD)(_li.HighPart), (DWORD)(_li.LowPart), NULL);
	if (hFileMap == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFileMapping Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		return dwErrorReturnValue;
	}
	CloseHandle(hFile);
	
	_li.QuadPart = _offset;
	LPBYTE lpFile = (LPBYTE)MapViewOfFile(hFileMap, dwAccess, (DWORD)(_li.HighPart), (DWORD)(_li.LowPart), _size);
	if (lpFile == NULL)
	{
		TRACE("MapViewOfFile Fail, File Path : %s, Error Code : %d \n", _strFilePath, GetLastError());
		CloseHandle(hFileMap);
		return dwErrorReturnValue;
	}
	
	Copy(lpFile, _buf, _size);
	
	UnmapViewOfFile(lpFile);
	CloseHandle(hFileMap);
	return _size;
}

/***************************************************************************/
/********************以下是CFileModel的内部方法*****************************/
/***************************************************************************/

void CFileModel::Copy(void *lpFile, void *_buf, DWORD _size)
{

}
