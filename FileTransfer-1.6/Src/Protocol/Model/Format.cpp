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
 * 函数名称 : ToString
 * 函数功能 : 格式化Size类型数据为CString形式
 * 入口参数 : Size _size, Size类型数据, int _radix, 转化进制的基数
 * 出口参数 : CString形式的Size类型数据
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
 * 函数名称 : ToString
 * 函数功能 : 格式化速度信息为CString形式
 * 入口参数 : Size _speed, 速度, long _ratio, 参考比率
 * 出口参数 : CString形式的速度信息
 ---------------------------------------------------------------------------*/
CString CFormat::ToString(Size _speed, long _ratio)
{
	CString _strSpeed;
	CString _strEx;
	
	if (_speed <= 0)
	{
		return _T("");
	}
	else if (_speed < KB)		//字节
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
 * 函数名称 : ToString
 * 函数功能 : 获取剩余时间的CString形式
 * 入口参数 : Size _remainedTime, 剩余时间信息
 * 出口参数 : 剩余时间信息的CString形式
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
 * 函数名称 : FormatFileSize
 * 函数功能 : 格式化文件大小为指定格式的CString形式
 * 入口参数 : Size _fileSize, 文件大小
 * 出口参数 : 指定格式的CString形式
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
 * 函数名称 : Format
 * 函数功能 : 格式化时间信息为00 - 60之间的字符串信息
 * 入口参数 : Size _time, Size形式的时间信息，可以为时、分、秒
 * 出口参数 : 字符串形式的时间信息
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
