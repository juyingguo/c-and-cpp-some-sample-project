// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>

#include "MainFrm.h"
#include "StatisticsViewDlg.h"
#include "SelectCaptureDeviceDlg.h"
#include "NSPlayer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  WM_START_STREAM         WM_USER + 104
#define  WM_STOP_STREAM          WM_USER + 105

ULONG  StartTime = 0;
BOOL   g_border = 1;


///////////////////////////////////////////////////////////////////////////////
//[运行文件名],/rec,主进程窗口句柄

#define TimerTicker		100

extern HANDLE m_hBeginRecord = INVALID_HANDLE_VALUE;
extern HANDLE m_hStopRecord = INVALID_HANDLE_VALUE;
extern HANDLE m_hCloseEvent = INVALID_HANDLE_VALUE;

BOOL	m_bNeedRecord =FALSE;

HWND MyWnd;

char *p,*pt;

char tmps[1024],cfgwjm[1024],rundir[1024],tmpwjm[1024];
char jgpath[1024];

CString pszFileName,psztmp;
int nIdx,index;

char camdes[1024],micdes[1024];

int selflag;

///////////////////////////////////////////////////////////////////////////////

CMainFrame         *  gpMainFrame = NULL;


//LRESULT CALLBACK OnDisplayVideo(int devNum,PBYTE pRgb, int dwSize); //图像RGB24数据回调

//采集到的视频图像回调
LRESULT CALLBACK VideoCaptureCallback(AVStream * input_st, enum PixelFormat pix_fmt, AVFrame *pframe, INT64 lTimeStamp);

//采集到的音频数据回调
LRESULT CALLBACK AudioCaptureCallback(AVStream * input_st, AVFrame *pframe, INT64 lTimeStamp);

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
    ON_COMMAND_RANGE(32770,32880, OnClickMenu)
    ON_UPDATE_COMMAND_UI_RANGE(32770, 32880, OnUpdateMenuUI)
	ON_MESSAGE(WM_START_STREAM, OnStartStream)
	ON_MESSAGE(WM_STOP_STREAM, OnStopStream)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_ENTERIDLE()

	ON_MESSAGE(MESSAGE_START_RECORD,StartRec)
	ON_MESSAGE(MESSAGE_STOP_RECORD,StopRec)

END_MESSAGE_MAP()


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	gpMainFrame = this;
   
	m_bPreview  = TRUE;
	m_nFPS = 0;
		                 
	m_fp = INVALID_HANDLE_VALUE;

	 /* register all codecs, demux and protocols */
    avcodec_register_all();
    av_register_all();

	avdevice_register_all(); 

	/////////////////////////////////////////////////////////////////////
	selflag=0;
	ntimer=autotimer=timertst=0;
	nIdx = 0;

	char *pc;
	GetModuleFileName(NULL,rundir,1000);
	strcpy_s(cfgwjm,1000,rundir);
	_strupr_s(rundir);
	pc=cfgwjm+strlen(cfgwjm)-1;
	while( (pc>cfgwjm) && (*pc!='.') ) pc--;
	if(*pc=='.')
	{
		pc++;	*pc='\0';
		strcat_s(cfgwjm,1000,"ini");
	}

	pc=rundir+strlen(rundir)-1;
	while( (pc>rundir) && (*pc!='\\') && (*pc!=':') ) pc--;
	if(*pc==':')
	{
		pc++;	*pc='\\';
	}
	if(*pc=='\\')
	{
		pc++;	*pc='\0';
	}
	/////////////

	CreateDirectory("d:\\DATA",NULL);
	strcpy_s(jgpath,1000,"d:\\DATA");
	/////////////////////////////////////////////////////////////////////
}

CMainFrame::~CMainFrame()
{

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView.Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
		CRect(0, 0, 0, 0), this, -1))
	{
		//TRACE0("未能创建视频预览窗口\n");
		return -1;
	}
	

    SetWindowPos(NULL, 0, 0, 400, 350, SWP_NOZORDER|SWP_NOMOVE);
	

	//OnStartStream(0, 0);

	m_strAppTitle = GetTitle();

	MyWnd=GetSafeHwnd();
	if(MyWnd==NULL) MyWnd=this->m_hWnd;

	///////////////////////////////////////////////////////////////////////////////////
	m_hBeginRecord = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hStopRecord = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hCloseEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	////////摄像头描述
	strcpy_s(camdes,1000,"");
	strcpy_s(micdes,1000,"");

	///////在配置文件中没有找到录像摄像头描述时
	GetPrivateProfileString("录像回放配置","录像摄像头名称","",camdes,1000,cfgwjm);
	GetPrivateProfileString("录像回放配置","录音通道名称","",micdes,1000,cfgwjm);
	GetPrivateProfileString("录像回放配置","视频文件保存路径","",jgpath,1000,cfgwjm);
	if(strlen(jgpath)<3)
	{
		strcpy(jgpath,"d:\\data");
		////////////写入信息/////////
		WritePrivateProfileString("录像回放配置","视频文件保存路径",jgpath,cfgwjm);
	}

	strcpy(tmps,jgpath);
	pt=tmps;
	while( (int)(pt-tmps)<(int)strlen(jgpath) )
	{
		if( (*pt!='\\') && (*pt!='\0') )
		{
			pt++;
			continue;
		}
		if(*pt=='\\')
		{
			if( *(pt-1)!=':')
			{
				*pt='\0';
				CreateDirectory(tmps,NULL);
				*pt='\\';
			}
		}
		pt++;
	}
	CreateDirectory(jgpath,NULL);
	if(jgpath[strlen(jgpath)-1]=='\\') jgpath[strlen(jgpath)-1]='\0';

	selflag=5;
	if( (strlen(camdes)<1) || (strlen(micdes)<1) )
	{
		selflag=0;
		ntimer=::SetTimer(this->m_hWnd,1,TimerTicker,NULL);
	}

	if(selflag>1)
	{
		selflag=5;
		CSelectCaptureDeviceDlg dlg;
		dlg.DoModal();
	}

	if( (strlen(camdes)<1) || (strlen(micdes)<1) )
	{
		selflag=0;
		ntimer=::SetTimer(this->m_hWnd,1,TimerTicker,NULL);
	}

	AfxBeginThread((AFX_THREADPROC)ThreadRecordControl,NULL);

	return 0;
}


UINT __stdcall CMainFrame::ThreadRecordControl(LPVOID pParameter)
{
	HANDLE hHandle[5];
	int handnum;

	handnum=0;
	if( (m_hBeginRecord!=NULL) && (m_hBeginRecord!=INVALID_HANDLE_VALUE) )
	{
		hHandle[handnum] = m_hBeginRecord;
		handnum++;
	}
	if( (m_hStopRecord!=NULL) && (m_hStopRecord!=INVALID_HANDLE_VALUE) )
	{
		hHandle[handnum] = m_hStopRecord;
		handnum++;
	}
	if( (m_hCloseEvent!=NULL) && (m_hCloseEvent!=INVALID_HANDLE_VALUE) )
	{
		hHandle[handnum] = m_hCloseEvent;
		handnum++;
	}

	psztmp.Format("\\%d.",nIdx);
	pszFileName = jgpath+psztmp;

	psztmp=pszFileName;
	pszFileName=psztmp+"mkv";

	if(GetFileAttributes(pszFileName)!=-1) DeleteFile(pszFileName);

	if(handnum==0) return 0;
	while(TRUE)
	{
		if(handnum>1)
		{
			index= WaitForMultipleObjects(handnum,hHandle,FALSE,INFINITE);
		}
		else if(handnum==1)
		{
			index= WaitForSingleObject(hHandle[0],INFINITE);
		}
		if(index==WAIT_FAILED)
		{
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,0,NULL);
			// Process any inserts in lpMsgBuf.
			// ...
			// Display the string.
			AfxMessageBox((LPCTSTR)lpMsgBuf,MB_SYSTEMMODAL|MB_OKCANCEL);
			// Free the buffer.
			LocalFree( lpMsgBuf );
		}
		if(hHandle[index-WAIT_OBJECT_0]==m_hBeginRecord)
		{
			ResetEvent(hHandle[index-WAIT_OBJECT_0]);
			::SendMessage(MyWnd,MESSAGE_START_RECORD,0,0);
			continue;
		}
		if(hHandle[index-WAIT_OBJECT_0]==m_hStopRecord)
		{
			ResetEvent(hHandle[index-WAIT_OBJECT_0]);
			::SendMessage(MyWnd,MESSAGE_STOP_RECORD,0,0);
			Sleep(50);

			nIdx++;
			psztmp.Format("\\%d.",nIdx);
			pszFileName = jgpath+psztmp;

			psztmp=pszFileName;
			pszFileName=psztmp+"mkv";

			if(GetFileAttributes(pszFileName)!=-1) DeleteFile(pszFileName);
			Sleep(50);

			continue;
		}
		if(hHandle[index-WAIT_OBJECT_0]==m_hCloseEvent)
		{
			if(m_bNeedRecord)
			{
				::PostMessage(MyWnd,MESSAGE_STOP_RECORD,0,0);
			}
			m_bNeedRecord = FALSE;
			ResetEvent(hHandle[index-WAIT_OBJECT_0]);	

			Sleep(300);
			//AfxGetMainWnd()->PostMessage(WM_QUIT,0,0);
			::PostQuitMessage(0);

			return 0;
		}
	}
	return 0;
}

void CMainFrame::OnDestroy()
{
	if(m_bNeedRecord)
	{
		StopRec(0,0);
	}

	if(ntimer!=0)
	{
		::KillTimer(this->m_hWnd,ntimer);
		ntimer=0;
	}

	if(autotimer!=0)
	{
		::KillTimer(this->m_hWnd,autotimer);
		autotimer=0;
	}

	Sleep(100);
	
	CFrameWnd::OnDestroy();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN;

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


void CMainFrame::OnClickMenu(UINT uMenuId)
{
	switch (uMenuId)
	{
		case IDM_START_CAP: //开始采集
		{
			if(!m_bNeedRecord)
			{
				SetEvent(m_hBeginRecord);
				SetWindowText(_T("正在采集..."));
			}
		}
		break;
		
		case IDM_STOP_CAP: //停止采集
		{
			if(m_bNeedRecord)
			{
				SetEvent(m_hStopRecord);
				SetWindowText(m_strAppTitle);
			}
		}
		break;

		case IDM_SEL_DEVICE: //选择设备
		{
			selflag=0;
			CSelectCaptureDeviceDlg dlg;
			dlg.DoModal();
			if( (strlen(camdes)>2) && (strlen(micdes)>2) )
			{
				////////////写入信息/////////
				WritePrivateProfileString("录像回放配置","录像摄像头名称",camdes,cfgwjm);
				WritePrivateProfileString("录像回放配置","录音通道名称",micdes,cfgwjm);
				WritePrivateProfileString("录像回放配置","视频文件保存路径",jgpath,cfgwjm);
			}
		}
		break;
		
		case IDM_PREVIEW_VIDEO: //预览
		{
			m_bPreview = !m_bPreview;
		}
        break;

	}//switch	 
}


void CMainFrame:: OnUpdateMenuUI(CCmdUI* pCmdUI)
{
  switch(pCmdUI->m_nID)
 {
  case IDM_SET_NOBORDER:
	  {
        pCmdUI->SetCheck(!g_border);
	  }
	  break;

  case IDM_PREVIEW_VIDEO: //是否预览
	 {
		 pCmdUI->SetCheck(m_bPreview); 
	 }
	 break;

 }//switch

}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	m_wndView.MoveWindow(rect);
}


void CMainFrame::OnTimer(UINT nIDEvent)
{
	CFrameWnd::OnTimer(nIDEvent);
	if( (nIDEvent==ntimer) && (ntimer!=0) )
	{
		::KillTimer(this->m_hWnd,ntimer);
		ntimer=0;
		SelDevice();
	}
	if( (nIDEvent==autotimer) && (autotimer!=0) )
	{
		::KillTimer(this->m_hWnd,autotimer);
		autotimer=0;
		AutoRec();
	}

}

LRESULT  CMainFrame:: OnStartStream(WPARAM wParam, LPARAM lParam)
{
	m_InputStream.SetVideoCaptureCB(VideoCaptureCallback);
	m_InputStream.SetAudioCaptureCB(AudioCaptureCallback);

	bool bRet;

	bRet = m_InputStream.OpenInputStream(); //初始化采集设备
	if(!bRet)
	{
		MessageBox(_T("打开采集设备失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		return 1;
	}

	int cx, cy, ifps;
	AVPixelFormat pixel_fmt;

	if(m_InputStream.GetVideoInputInfo(cx, cy, ifps, pixel_fmt)) //获取视频采集源的信息
	{
		m_OutputStream.SetVideoCodecProp(AV_CODEC_ID_H264, ifps, 500000, 100, cx, cy); //设置视频编码器属性
	}

    int sample_rate = 0, channels = 0;
	AVSampleFormat  sample_fmt;

	if(m_InputStream.GetAudioInputInfo(sample_fmt, sample_rate, channels)) //获取音频采集源的信息
	{
		m_OutputStream.SetAudioCodecProp(AV_CODEC_ID_AAC, sample_rate, channels, 32000); //设置音频编码器属性
	}

	bRet  = m_OutputStream.OpenOutputStream(pszFileName);	//设置输出文件名
	if(!bRet)
	{
		MessageBox(_T("初始化输出失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		return 1;
	}

	m_Painter.SetVideoWindow(m_wndView.GetSafeHwnd()); //设置视频预览窗口
	m_Painter.SetSrcFormat(cx, cy, 24, TRUE); //第4个参数表示是否翻转图像
	m_Painter.SetStretch(TRUE); //是否缩放显示图像
	m_Painter.Open();

	bRet = m_InputStream.StartCapture(); //开始采集

	StartTime = timeGetTime();

	m_frmCount = 0;
	m_nFPS = 0;

	return 0;
}

LRESULT  CMainFrame:: OnStopStream(WPARAM wParam, LPARAM lParam)
{
	m_InputStream.CloseInputStream();
	m_OutputStream.CloseOutput();
    m_Painter.Close();

	StartTime = 0;

	return 0;
} 


//采集到的视频图像回调
LRESULT CALLBACK VideoCaptureCallback(AVStream * input_st, enum PixelFormat pix_fmt, AVFrame *pframe, INT64 lTimeStamp)
{
	if(gpMainFrame->IsPreview())
	{
	   gpMainFrame->m_Painter.Play(input_st, pframe);
	}

	gpMainFrame->m_OutputStream.write_video_frame(input_st, pix_fmt, pframe, lTimeStamp);
	return 0;
}

//采集到的音频数据回调
LRESULT CALLBACK AudioCaptureCallback(AVStream * input_st, AVFrame *pframe, INT64 lTimeStamp)
{
	gpMainFrame->m_OutputStream.write_audio_frame(input_st, pframe, lTimeStamp);
	return 0;
}

LRESULT CMainFrame::StartRec(WPARAM wParam, LPARAM lParam)
{
	if(m_bNeedRecord) return 0;

	m_bNeedRecord = TRUE;

	m_InputStream.SetVideoCaptureDevice(camdes);
	m_InputStream.SetAudioCaptureDevice(micdes);

	LRESULT lResult = OnStartStream(0, 0);

	return 1;
}

LRESULT CMainFrame::StopRec(WPARAM wParam, LPARAM lParam)
{
	if(!m_bNeedRecord) return 0;

	m_bNeedRecord = false;

	OnStopStream(0, 0);

	{
		char str[1024];
		sprintf(str,"已经生成文件:\n%s",pszFileName);
		MessageBox(str,"提示",MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
	}

	return 1;
}
void CMainFrame::AutoRec()
{
	int selbk;

	selbk=selflag;
	selflag=5;

	CSelectCaptureDeviceDlg dlg;

	dlg.DoModal();
	selflag=selbk;
	{
		m_InputStream.SetVideoCaptureDevice(camdes);
		m_InputStream.SetAudioCaptureDevice(micdes);

		pszFileName=jgpath;
		pszFileName += "\\000.MKV";
		DeleteFile(pszFileName);

		LRESULT lResult = OnStartStream(0, 0);

		if(lResult == 0)
		{
			sprintf(tmps,"正在采集1...%s",pszFileName);
			SetWindowText(tmps);
		}
	}
	Sleep(10000);

	{
		OnStopStream(0, 0);
		SetWindowText(m_strAppTitle);
	}

	pszFileName=jgpath;
	pszFileName += "\\001.MKV";
	DeleteFile(pszFileName);

	LRESULT lResult = OnStartStream(0, 0);

	if(lResult == 0)
	{
		sprintf(tmps,"正在采集2...%s",pszFileName);
		SetWindowText(tmps);
	}

	Sleep(10000);

	{
		OnStopStream(0, 0);
		SetWindowText(m_strAppTitle);
	}

	PostMessage(WM_QUIT,0,0);
	PostMessage(WM_CLOSE,0,0);
}

void CMainFrame::SelDevice()
{
	CSelectCaptureDeviceDlg dlg;
	dlg.DoModal();
	if( (strlen(camdes)>2) && (strlen(micdes)>2) )
	{
		////////////写入信息/////////
		WritePrivateProfileString("录像回放配置","录像摄像头名称",camdes,cfgwjm);
		WritePrivateProfileString("录像回放配置","录音通道名称",micdes,cfgwjm);
		WritePrivateProfileString("录像回放配置","视频文件保存路径",jgpath,cfgwjm);
	}
}