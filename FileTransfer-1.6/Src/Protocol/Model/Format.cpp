// Format.cpp: implementation of the CFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Format.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormat::CFormat()
{

}

CFormat::~CFormat()
{

}
/*---------------------------------------------------------------------------
 * �������� : ToString
 * �������� : ��ʽ��Size��������ΪCString��ʽ
 * ��ڲ��� : Size _size, Size��������, int _radix, ת�����ƵĻ���
 * ���ڲ��� : CString��ʽ��Size��������
 ---------------------------------------------------------------------------*/
CString CFormat::ToString(Size _size, int _radix)
{
	char _szSize[MAX_BUF_LEN];
	int i = 0;
	while (_size)
	{
		char c = (char)((_size % _radix) + 0x30);
		_szSize[i++] = c;
		_size = _size / _radix;
	}
	_szSize[i] = '\0';
	CString _strSize = _szSize;
	_strSize.MakeReverse();
	return _strSize;
}
/*---------------------------------------------------------------------------
 * �������� : ToString
 * �������� : ��ʽ���ٶ���ϢΪCString��ʽ
 * ��ڲ��� : Size _speed, �ٶ�, long _ratio, �ο�����
 * ���ڲ��� : CString��ʽ���ٶ���Ϣ
 ---------------------------------------------------------------------------*/
CString CFormat::ToString(Size _speed, long _ratio)
{
	CString _strSpeed;
	CString _strEx;
	
	if (_speed <= 0)
	{
		return _T("");
	}
	else if (_speed < KB)		//�ֽ�
	{
		_speed = _speed * _ratio;
		_strSpeed = CFormat::ToString(_speed, RADIX);
		_strEx = _T(" B / s");
	}
	else if ( (_speed >= KB) && (_speed < MB) )		//KB
	{
		_speed = _speed / KB;
		_speed = _speed * _ratio;
		_strSpeed = CFormat::ToString(_speed, RADIX);
		_strEx = _T(" K / s");
	}
	else
	{
		_speed = _speed / MB;
		_speed = _speed * _ratio;
		_strSpeed = CFormat::ToString(_speed, RADIX);
		_strEx = _T(" M / s");
	}
	_strSpeed = _strSpeed + _strEx;
	return _strSpeed;
}
/*---------------------------------------------------------------------------
 * �������� : ToString
 * �������� : ��ȡʣ��ʱ���CString��ʽ
 * ��ڲ��� : Size _remainedTime, ʣ��ʱ����Ϣ
 * ���ڲ��� : ʣ��ʱ����Ϣ��CString��ʽ
 ---------------------------------------------------------------------------*/
CString CFormat::ToString(Size _remainedTime)
{
	CString _strRemainedTime;
	Size _hour, _minute, _second;
	_second = _remainedTime % TIME_UNIT;
	_remainedTime = _remainedTime / TIME_UNIT;
	_minute = _remainedTime % TIME_UNIT;
	_remainedTime = _remainedTime / TIME_UNIT;
	_hour = _remainedTime % TIME_UNIT;
	
	CString _strHour, _strMinute, _strSecond;
	_strHour	= CFormat::Format(_hour);
	_strMinute	= CFormat::Format(_minute);
	_strSecond	= CFormat::Format(_second);
	
	_strRemainedTime = _strHour;
	_strRemainedTime = _strRemainedTime + _T(" : ");
	_strRemainedTime = _strRemainedTime + _strMinute;
	_strRemainedTime = _strRemainedTime + _T(" : ");
	_strRemainedTime = _strRemainedTime + _strSecond;
	return _strRemainedTime;
}
/*---------------------------------------------------------------------------
 * �������� : FormatFileSize
 * �������� : ��ʽ���ļ���СΪָ����ʽ��CString��ʽ
 * ��ڲ��� : Size _fileSize, �ļ���С
 * ���ڲ��� : ָ����ʽ��CString��ʽ
 ---------------------------------------------------------------------------*/
CString CFormat::FormatFileSize(Size _fileSize)
{
	CString _strSize;
	CString _strEx;
	if ( _fileSize < KB)
	{
		_strSize.Format("%d", _fileSize);
		_strEx = _T(" B");
	}
	else
	{
		double _ratio;
		if (_fileSize >= KB && _fileSize < MB)
		{
			_ratio = (double)(KB);
			_strEx = _T(" KB");
		}
		else if (_fileSize >= MB && (double)(_fileSize) < GB)
		{
			_ratio = (double)(MB);
			_strEx = _T(" MB");
		}
		else if ((double)(_fileSize) >= GB && (double)(_fileSize) < TB)
		{
			_ratio = (double)(GB);
			_strEx = _T(" GB");
		}
		else
		{
			_ratio = (double)(TB);
			_strEx = _T(" TB");
		}
		
		double _dSize = (double)(_fileSize);
		_dSize = _dSize / _ratio;
		_strSize.Format("%.2lf", _dSize);
	}
	_strSize = _strSize + _strEx;
	return _strSize;
}

/*---------------------------------------------------------------------------
 * �������� : Format
 * �������� : ��ʽ��ʱ����ϢΪ00 - 60֮����ַ�����Ϣ
 * ��ڲ��� : Size _time, Size��ʽ��ʱ����Ϣ������Ϊʱ���֡���
 * ���ڲ��� : �ַ�����ʽ��ʱ����Ϣ
 ---------------------------------------------------------------------------*/
CString CFormat::Format(Size _time)
{
	CString _strTime;
	if (_time <= 0)
	{
		_strTime = _T("00");
	}
	else if (_time > 0 && _time < 10)
	{
		_strTime = _T("0");
		CString _strTemp = CFormat::ToString(_time, RADIX);
		_strTime = _strTime + _strTemp;
	}
	else
	{
		_strTime = CFormat::ToString(_time, RADIX);
	}
	return _strTime;
}
