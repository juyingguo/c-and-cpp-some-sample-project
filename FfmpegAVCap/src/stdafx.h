// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

#define  WINVER 0x0501


#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0501 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0501	//为 IE 5.0 及更新版本改变为适当的值。
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"

typedef struct _decode_statis_
{
	UINT  nVideoFormat; //1--MPEG1, 2--MPEG2, 3--MPEG4, 4--H264
   	UINT  nVideoFrames;
	UINT  nAudioFrames;
    UINT  nFramerate;
	int  nWidth;
	int  nHeight;
    DWORD dwBPS; 
}DECODE_STATIS;



//#include <afxsock.h>		// MFC socket extensions
#include <Winsock2.h>
#include "./include/stdint.h"

#include <atltrace.h>

//#include <streams.h>
#include <string>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif 

#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

#include "./include/libavcodec/avcodec.h"
#include "./include/libavutil/mathematics.h"
#include "./include/libavutil/avutil.h"
#include "./include/libswscale/swscale.h"
#include "./include/libavutil/fifo.h"
#include "./include/libavformat/avformat.h"
#include "./include/libavutil/opt.h"
#include "./include/libavutil/error.h"
#include "./include/libswresample/swresample.h"
#include "./include/libavutil/audio_fifo.h"
#include "./include/libavutil/time.h"
#include "./include/libavdevice/avdevice.h"
#ifdef __cplusplus
}
#endif

#pragma comment( lib, "avcodec.lib")
#pragma comment( lib, "avutil.lib")
#pragma comment( lib, "avformat.lib")
#pragma comment(lib,   "swresample.lib")
#pragma comment(lib,  "swscale.lib" )
#pragma comment(lib, "avdevice.lib")

#pragma warning(disable:4996)

#ifndef CodecID
#define CodecID AVCodecID
#endif

#define _USE_NEW_FFMPEG_API_

#define nullptr NULL

