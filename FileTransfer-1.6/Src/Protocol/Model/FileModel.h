/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : FileModel.h
 * �ļ��汾 : 2.0
 * ������Ա : Yao
 * �������� : 2010.03.08
 * ����ԭ�� : �����ļ�����ģ��Ϊ�ڴ��ļ�ӳ�䷽ʽ
 * �ļ�˵�� : interface for the CFileModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMODEL_H__38BD225D_E0E9_429B_828C_171077800B7F__INCLUDED_)
#define AFX_FILEMODEL_H__38BD225D_E0E9_429B_828C_171077800B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

/*
 * ������� : CFileModel
 * ������; : �����ļ���д�Ļ���ģ��
 */

/*ʹ��ģ����Ҫע���Ҫ��
1. ģ���ṩ���ַ�ʽ����д�ļ� : 
   һ���������ṩ��ȫ�־�̬����Read��Write����, ���ô˷������Ի�ñȽϸߵĲ���Ч��; 
   һ��������ģ�巽��ģʽʵ�ֵĽӿ�Serialize���˷�ʽ��Ҫ�������ļ�ģ��
   ���廯ΪCFileReadModel��CFileWriteModel���������Զ�����ļ�ģ�Ͷ���, 
   Ȼ�����Serialize����, �˷�ʽ���бȽϸߵ���չ�Ժ�ά����. 
2. ����Serialize�ӿ��������ļ�����Ҫ��������廯�Ķ������������������ : 
   2.1 CreateFile()������Ҫ�õ��Ĳ���
       DWORD dwDesiredAccess : 
	         CFileReadModel		: GENERIC_READ
	         CFileWriteModel	: GENERIC_WRITE|GENERIC_READ
       DWORD dwShareMode : 
	         CFileReadModel		: FILE_SHARE_READ
			 CFileWriteModel	: FILE_SHARE_WRITE
       DWORD dwCreationDisposition : 
	         CFileReadModel		: OPEN_EXISTING
			 CFileWriteModel	: OPEN_ALWAYS
   2.2 ��������ķ���ֵ
       DWORD dwErrorReturnValue : 
	         CFileReadModel		: READ_ERROR
			 CFileWriteModel	: WRITE_ERROR
   2.3 CreateFileMapping()������Ҫ�õ��Ĳ���
       DWORD flProtect : 
	         CFileReadModel		: PAGE_READONLY
			 CFileWriteModel	: PAGE_READWRITE
   2.4 MapViewOfFile()������Ҫ�õ��Ĳ���
       DWORD dwAccess : 
	         CFileReadModel		: FILE_MAP_READ
			 CFileWriteModel	: FILE_MAP_WRITE
3. ����Serialize�ӿڲ����ļ�, ��Ҫ����������ʵ��Copy()���� : 
   CFileReadModel	: memcpy(_buf, lpFile, _size)
   CFileWriteModel	: memcpy(lpFile, _buf, _size)
*/
class CFileModel  
{
public:
	/***************************************************************************/
	/********************������CFileModel�����ṩ�Ľӿ�*************************/
	/***************************************************************************/
	static Size GetSize(CString _strFilePath);					//��ȡ�ļ���С
	static DWORD Read(CString _strFilePath, Size _fileSize, void *_buf, Size _offset, DWORD _size);
																//��ָ��·���ļ���ȡָ��ƫ�ƴ���ָ����С���ļ���
	static DWORD Write(CString _strFilePath, Size _fileSize, const void *_buf, Size _offset, DWORD _size);
																//������������д��ָ��·�����ļ���ָ��ƫ��λ�ô�
	DWORD Serialize(void * _buf, Size _offset, DWORD _size);	//���л������ļ�

	CFileModel();
	virtual ~CFileModel();
protected:
	/***************************************************************************/
	/********************������CFileModel���ڲ�����*****************************/
	/***************************************************************************/
protected:
	virtual void Copy(void * lpFile, void * _buf, DWORD _size);	//�������̣�������ʵ����
	/***************************************************************************/
	/*******************���¶���CFileModel�Ļ�������****************************/
	/***************************************************************************/
	CString _strFilePath;										//�ļ�·��
	Size _fileSize;												//�ļ���С

	/********************������ģ��ģʽ��Ҫʵ�����Ĳ���*************************/
	DWORD dwDesiredAccess;										//CreateFile(); GENERIC_READ - GENERIC_WRITE|GENERIC_READ
	DWORD dwShareMode;											//CreateFile(); FILE_SHARE_READ - FILE_SHARE_WRITE
	DWORD dwCreationDisposition;								//CreateFile(); OPEN_EXISTING - OPEN_ALWAYS
	DWORD dwErrorReturnValue;									//����ʱ�ķ���ֵ; READ_ERROR - WRITE_ERROR
	DWORD flProtect;											//CreateFileMapping(); PAGE_READONLY - PAGE_READWRITE
	DWORD dwAccess;												//MapViewOfFile(); FILE_MAP_READ - FILE_MAP_WRITE
};

#endif // !defined(AFX_FILEMODEL_H__38BD225D_E0E9_429B_828C_171077800B7F__INCLUDED_)
