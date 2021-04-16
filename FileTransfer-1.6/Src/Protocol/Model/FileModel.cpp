/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileModel.cpp
 * �ļ��汾 : 2.0
 * ������Ա : Yao
 * �������� : 2010.03.08
 * ����ԭ�� : �����ļ�������ģ��Ϊ�ڴ��ļ�ӳ��ķ�ʽ
 * �ļ�˵�� : implementation of the CFileModel class.
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
/********************������CFileModel�����ṩ�Ľӿ�*************************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : GetSize
 * �������� : ��ȡ�ļ���С
 * ��ڲ��� : ��
 * ���ڲ��� : �ļ���С
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
 * �������� : Read
 * �������� : ��ָ��·���ļ���ȡָ��ƫ�ƴ���ָ����С���ļ���
 * ��ڲ��� : CString _strFilePath, �ļ�·��;
			  Size _fileSize, �ļ���С
			  void *_buf, �ļ���ȡ������
			  Size _offset, ��ȡ��ƫ�ƴ�
			  DWORD _size, ָ���Ķ�ȡ�����ݴ�С
 * ���ڲ��� : ��ȷ����ʵ�ʶ�ȡ�ĳ���, ���򷵻�READ_ERROR
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
 * �������� : Write
 * �������� : ������������д��ָ��·�����ļ���ָ��ƫ��λ�ô�
 * ��ڲ��� : CString _strFilePath, �ļ�·��;
			  Size _fileSize, �ļ���С
			  void *_buf, ��Ҫ�������ݵĻ�����
			  Size _offset, д���ƫ�ƴ�
			  DWORD _size, ָ����д������ݴ�С
 * ���ڲ��� : ��ȷ����ʵ��д������ݵĴ�С; ���򷵻�FALSE
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
 * �������� : Serialize
 * �������� : ��ָ���Ļ������������л�����
 * ��ڲ��� : void * _buf, �������ļ��Ļ�����, ���ģ�͹�����Ч��;
			  Size _offset, ������ƫ�����;
			  DWORD _size, �������Ĵ�С
 * ���ڲ��� : ������READ_ERROR, ��ȷ����ʵ�ʲ����ĳ���_size
 * �������� : 2010.03.08
 * ����ԭ�� : �����ļ��ķ�ʽ����Ϊ�ڴ��ļ�ӳ��
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
/********************������CFileModel���ڲ�����*****************************/
/***************************************************************************/

void CFileModel::Copy(void *lpFile, void *_buf, DWORD _size)
{

}
