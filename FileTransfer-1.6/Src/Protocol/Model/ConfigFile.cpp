/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : ConfigFile.cpp
 * �ļ��汾 : 2.1
 * ������Ա : Yao
 * �������� : 2010.02.24
 * ����ԭ�� : �����������������ļ�ʧ�ܵ�Bug
			  ����������ĺϷ����߼��ж�
 * �������� : 2010.03.08
 * ����ԭ�� : �ṩ������Ϸ����жϵ�API
 * �ļ�˵�� : implementation of the CConfigFile class.
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
/****************���¶��������ļ��������ṩ�ķ��ʽӿ�*********************/
/***************************************************************************/

/****************���¶��������ļ�������Ե�set��get����*********************/

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
 * �������� : Judge
 * �������� : ί��CValidJudge�������������кϷ����ж�
 * ��ڲ��� : CString _strConfig, ����������; int _max, int _min, �����Χ
 * ���ڲ��� : ����CValidJudge�����Ӧ��Judge������ֵ
 ---------------------------------------------------------------------------*/
int CConfigFile::Judge(CString _strConfig, int _max, int _min)
{
	CValidJudge _valid(_strConfig, _max, _min);
	return _valid.Judge();
}

/*---------------------------------------------------------------------------
 * �������� : Load
 * �������� : ���������ļ�����Ϣ
 * ��ڲ��� : ��
 * ���ڲ��� : ��ȡ�Ľ����TRUE����ɹ���FALSE����ʧ��
 * �������� : 2010.01.23
 * ����ԭ�� : �ں������ش����ӹر��ļ�����䣬�����������������ļ�ʧ�ܵ�Bug
 ---------------------------------------------------------------------------*/
int CConfigFile::Load()
{
	FILE * fp;
	if((fp = fopen(_strFilePath,"r"))==NULL)
	{
		return FALSE;
	}
	//��ȡ��������Ŀ��Ϣ�����кϷ����ж�
	char _temp[MAX_BUF_LEN];
	fgets(_temp,MAX_BUF_LEN,fp);
	CString _strLoad = _temp;
	
	if (Judge(_strLoad) == FALSE)
	{
		fclose(fp);						///CLOSE
		return FALSE;
	}

	_length = atoi(_strValue[INDEX_CONFIG_ITEM_NUM]);

	//���ݶ�ȡ����������Ŀ��Ϣ�������е�������
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
/*                ���¶��������ļ�����ڲ�����                             */
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : InitFilePath
 * �������� : ��ʼ���ļ�·��Ϊ��ǰ����·��
 * ��ڲ��� : ��
 * ���ڲ��� : ��
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
 * �������� : GetItem
 * �������� : ���ݶ�ȡ����������Ϣ��ȡ�����������
 * ��ڲ��� : CString _strLoad����ȡ����������Ϣ
 * ���ڲ��� : �����������
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
 * �������� : GetValue
 * �������� : ���ݶ�ȡ����������Ϣ��ȡ�������ֵ
 * ��ڲ��� : CString _strOrginal����ȡ����������Ϣ
 * ���ڲ��� : �������ֵ
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
	for (int i = _index; i < _strLoad.GetLength()-1; i++)	//��ȥ"\r\n"
	{
		_temp[i-_index] = _strLoad.GetAt(i);
	}
	_temp[i-_index] = '\0';
	CString _str = _temp;
	return _str;
}
/*---------------------------------------------------------------------------
 * �������� : Judge
 * �������� : ������������ĺϷ��Խ����ж�
 * ��ڲ��� : CString _strLoad, �������������
 * ���ڲ��� : �Ϸ�����TRUE�����򷵻�FALSE
 ---------------------------------------------------------------------------*/
int CConfigFile::Judge(CString _strLoad)
{
	_strItem[INDEX_CONFIG_ITEM_NUM] = GetItem(_strLoad);
	//�ȱȽϵ�һ���Ƿ��Ԥ���������ƥ�䣬��ƥ����ֱ�ӷ���
	if (_strItem[INDEX_CONFIG_ITEM_NUM].Compare(CONFIGITEMNUM) != 0)
	{
		return FALSE;
	}
	//�ж϶�ȡ�������ļ��ĳ�����Ϣ�Ƿ���Ч����Ч��ֱ�ӷ���
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
