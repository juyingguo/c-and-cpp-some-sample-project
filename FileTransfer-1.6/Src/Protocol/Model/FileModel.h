/*******************************版权信息*************************************
 * 文件名称 : FileModel.h
 * 文件版本 : 2.0
 * 开发人员 : Yao
 * 更新日期 : 2010.03.08
 * 更新原因 : 调整文件操作模型为内存文件映射方式
 * 文件说明 : interface for the CFileModel class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMODEL_H__38BD225D_E0E9_429B_828C_171077800B7F__INCLUDED_)
#define AFX_FILEMODEL_H__38BD225D_E0E9_429B_828C_171077800B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/AppConfig.h"

/*
 * 类的名称 : CFileModel
 * 基本用途 : 定义文件读写的基本模型
 */

/*使用模型需要注意的要点
1. 模型提供两种方式来读写文件 : 
   一种是利用提供的全局静态函数Read和Write方法, 利用此方法可以获得比较高的操作效率; 
   一种是利用模板方法模式实现的接口Serialize，此方式需要将抽象文件模型
   具体化为CFileReadModel或CFileWriteModel或者其他自定义的文件模型对象, 
   然后调用Serialize方法, 此方式具有比较高的扩展性和维护性. 
2. 采用Serialize接口来操作文件，需要在子类具体化的对象参数包括如下内容 : 
   2.1 CreateFile()方法所要用到的参数
       DWORD dwDesiredAccess : 
	         CFileReadModel		: GENERIC_READ
	         CFileWriteModel	: GENERIC_WRITE|GENERIC_READ
       DWORD dwShareMode : 
	         CFileReadModel		: FILE_SHARE_READ
			 CFileWriteModel	: FILE_SHARE_WRITE
       DWORD dwCreationDisposition : 
	         CFileReadModel		: OPEN_EXISTING
			 CFileWriteModel	: OPEN_ALWAYS
   2.2 操作出错的返回值
       DWORD dwErrorReturnValue : 
	         CFileReadModel		: READ_ERROR
			 CFileWriteModel	: WRITE_ERROR
   2.3 CreateFileMapping()方法所要用到的参数
       DWORD flProtect : 
	         CFileReadModel		: PAGE_READONLY
			 CFileWriteModel	: PAGE_READWRITE
   2.4 MapViewOfFile()方法所要用到的参数
       DWORD dwAccess : 
	         CFileReadModel		: FILE_MAP_READ
			 CFileWriteModel	: FILE_MAP_WRITE
3. 采用Serialize接口操作文件, 需要在子类里面实现Copy()方法 : 
   CFileReadModel	: memcpy(_buf, lpFile, _size)
   CFileWriteModel	: memcpy(lpFile, _buf, _size)
*/
class CFileModel  
{
public:
	/***************************************************************************/
	/********************以下是CFileModel对外提供的接口*************************/
	/***************************************************************************/
	static Size GetSize(CString _strFilePath);					//获取文件大小
	static DWORD Read(CString _strFilePath, Size _fileSize, void *_buf, Size _offset, DWORD _size);
																//从指定路径文件读取指定偏移处的指定大小的文件块
	static DWORD Write(CString _strFilePath, Size _fileSize, const void *_buf, Size _offset, DWORD _size);
																//将缓冲区内容写入指定路径的文件的指定偏移位置处
	DWORD Serialize(void * _buf, Size _offset, DWORD _size);	//序列化操作文件

	CFileModel();
	virtual ~CFileModel();
protected:
	/***************************************************************************/
	/********************以下是CFileModel的内部方法*****************************/
	/***************************************************************************/
protected:
	virtual void Copy(void * lpFile, void * _buf, DWORD _size);	//拷贝过程，子类中实例化
	/***************************************************************************/
	/*******************以下定义CFileModel的基本属性****************************/
	/***************************************************************************/
	CString _strFilePath;										//文件路径
	Size _fileSize;												//文件大小

	/********************以下是模板模式所要实例化的参数*************************/
	DWORD dwDesiredAccess;										//CreateFile(); GENERIC_READ - GENERIC_WRITE|GENERIC_READ
	DWORD dwShareMode;											//CreateFile(); FILE_SHARE_READ - FILE_SHARE_WRITE
	DWORD dwCreationDisposition;								//CreateFile(); OPEN_EXISTING - OPEN_ALWAYS
	DWORD dwErrorReturnValue;									//出错时的返回值; READ_ERROR - WRITE_ERROR
	DWORD flProtect;											//CreateFileMapping(); PAGE_READONLY - PAGE_READWRITE
	DWORD dwAccess;												//MapViewOfFile(); FILE_MAP_READ - FILE_MAP_WRITE
};

#endif // !defined(AFX_FILEMODEL_H__38BD225D_E0E9_429B_828C_171077800B7F__INCLUDED_)
