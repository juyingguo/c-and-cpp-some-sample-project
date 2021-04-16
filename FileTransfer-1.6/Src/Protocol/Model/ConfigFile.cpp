/*******************************版权信息*************************************
 * 文件名称 : ConfigFile.cpp
 * 文件版本 : 2.1
 * 开发人员 : Yao
 * 更新日期 : 2010.02.24
 * 更新原因 : 修正连续载入配置文件失败的Bug
			  增加配置项的合法性逻辑判断
 * 更新日期 : 2010.03.08
 * 更新原因 : 提供配置项合法性判断的API
 * 文件说明 : implementation of the CConfigFile class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigFile.h"

#include "ValidJudge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigFile::CConfigFile()
{
	InitFilePath();
	_length	= INVALID_VALUE;
}

CConfigFile::~CConfigFile()
{

}

/***************************************************************************/
/****************以下定义配置文件类对外界提供的访问接口*********************/
/***************************************************************************/

/****************以下定义配置文件类的属性的set和get方法*********************/

CString CConfigFile::GetValue(int _index)
{
	if (_index < 0 || _index > _length-1)
	{
		return _T("");
	}
	return this->_strValue[_index];
}

CString CConfigFile::GetItem(int _index)
{
	if (_index < 0 || _index > _length-1)
	{
		return _T("");
	}
	return this->_strItem[_index];
}

int CConfigFile::GetLength()
{
	return this->_length;
}
/*---------------------------------------------------------------------------
 * 函数名称 : Judge
 * 函数功能 : 委托CValidJudge对象对配置项进行合法性判断
 * 入口参数 : CString _strConfig, 配置项名称; int _max, int _min, 配置项范围
 * 出口参数 : 返回CValidJudge对象对应的Judge方法的值
 ---------------------------------------------------------------------------*/
int CConfigFile::Judge(CString _strConfig, int _max, int _min)
{
	CValidJudge _valid(_strConfig, _max, _min);
	return _valid.Judge();
}

/*---------------------------------------------------------------------------
 * 函数名称 : Load
 * 函数功能 : 载入配置文件的信息
 * 入口参数 : 无
 * 出口参数 : 读取的结果，TRUE代表成功；FALSE代表失败
 * 更新日期 : 2010.01.23
 * 更新原因 : 在函数返回处增加关闭文件的语句，修正连续载入配置文件失败的Bug
 ---------------------------------------------------------------------------*/
int CConfigFile::Load()
{
	FILE * fp;
	if((fp = fopen(_strFilePath,"r"))==NULL)
	{
		return FALSE;
	}
	//提取配置项数目信息并进行合法性判断
	char _temp[MAX_BUF_LEN];
	fgets(_temp,MAX_BUF_LEN,fp);
	CString _strLoad = _temp;
	
	if (Judge(_strLoad) == FALSE)
	{
		fclose(fp);						///CLOSE
		return FALSE;
	}

	_length = atoi(_strValue[INDEX_CONFIG_ITEM_NUM]);

	//根据读取的配置项数目信息载入所有的配置项
	for (int i = 1; i < _length; i++)
	{
		_strLoad.Empty();
		memset(_temp,0,MAX_BUF_LEN);
		fgets(_temp,MAX_BUF_LEN,fp);
		_strLoad = _temp;
		_strItem[i] = GetItem(_strLoad);
		if (_strItem[i] == _T(""))
		{
			fclose(fp);					///CLOSE
			return FALSE;
		}
		_strValue[i] = GetValue(_strLoad);
		if (_strValue[i] == _T(""))
		{
			fclose(fp);					///CLOSE
			return FALSE;
		}
	}
	fclose(fp);							///CLOSE
	return TRUE;
}

/***************************************************************************/
/*                以下定义配置文件类的内部方法                             */
/***************************************************************************/

/*---------------------------------------------------------------------------
 * 函数名称 : InitFilePath
 * 函数功能 : 初始化文件路径为当前程序路径
 * 入口参数 : 无
 * 出口参数 : 无
 ---------------------------------------------------------------------------*/
void CConfigFile::InitFilePath()
{
	DWORD len = MAX_FILE_PATH_LEN;
	char curdir[MAX_FILE_PATH_LEN];
	DWORD total;
	total=GetCurrentDirectory(len,curdir);
	strcat(curdir,CONFIG_PATH);
	_strFilePath = curdir;
}

/*---------------------------------------------------------------------------
 * 函数名称 : GetItem
 * 函数功能 : 根据读取的配置项信息获取配置项的名称
 * 入口参数 : CString _strLoad，读取的配置项信息
 * 出口参数 : 配置项的名称
 ---------------------------------------------------------------------------*/
CString CConfigFile::GetItem(CString _strLoad)
{
	int _index = _strLoad.FindOneOf(FILTER);
	if (_index == -1)
	{
		return _T("");
	}
	char _temp[MAX_BUF_LEN];
	for (int i = 0; i < _index; i++)
	{
		_temp[i] = _strLoad.GetAt(i);
	}
	_temp[i] = '\0';
	CString _str = _temp;
	return _str;
}

/*---------------------------------------------------------------------------
 * 函数名称 : GetValue
 * 函数功能 : 根据读取的配置项信息获取配置项的值
 * 入口参数 : CString _strOrginal，读取的配置项信息
 * 出口参数 : 配置项的值
 ---------------------------------------------------------------------------*/
CString CConfigFile::GetValue(CString _strLoad)
{
	int _index = _strLoad.FindOneOf(FILTER);
	if (_index == -1)
	{
		return _T("");
	}
	_index++;
	char _temp[MAX_BUF_LEN];
	for (int i = _index; i < _strLoad.GetLength()-1; i++)	//除去"\r\n"
	{
		_temp[i-_index] = _strLoad.GetAt(i);
	}
	_temp[i-_index] = '\0';
	CString _str = _temp;
	return _str;
}
/*---------------------------------------------------------------------------
 * 函数名称 : Judge
 * 函数功能 : 对配置项首项的合法性进行判断
 * 入口参数 : CString _strLoad, 配置项首项参数
 * 出口参数 : 合法返回TRUE，否则返回FALSE
 ---------------------------------------------------------------------------*/
int CConfigFile::Judge(CString _strLoad)
{
	_strItem[INDEX_CONFIG_ITEM_NUM] = GetItem(_strLoad);
	//先比较第一项是否和预定义的名称匹配，不匹配则直接返回
	if (_strItem[INDEX_CONFIG_ITEM_NUM].Compare(CONFIGITEMNUM) != 0)
	{
		return FALSE;
	}
	//判断读取的配置文件的长度信息是否有效，无效则直接返回
	_strValue[INDEX_CONFIG_ITEM_NUM] = GetValue(_strLoad);
	if (_strValue[INDEX_CONFIG_ITEM_NUM] == _T(""))
	{
		return FALSE;
	}
	
	CValidJudge _valid(_strValue[INDEX_CONFIG_ITEM_NUM], MAX_CONFIG_ITEM, 0);
	if (_valid.Judge() == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
