// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include <afxmt.h>
#include <map>

#include "ChildView.h"
#include "VideoDisplayWnd.h"
#include "AVInputStream.h"
#include "AVOutputStream.h"
#include "ImagePainter.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////

#define MESSAGE_START_RECORD			WM_USER + 1301
#define MESSAGE_STOP_RECORD				WM_USER + 1302

static		HANDLE		m_hBeginRecord;
static		HANDLE		m_hStopRecord;
static		HANDLE		m_hCloseEvent;

////////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
    virtual ~CMainFrame();

public:
	BOOL   IsPreview() { return m_bPreview; }

	////////////////////////////////////////////////////////////////////////////////
	CString m_pszCmdLine;
	BOOL	m_bIsChild;

	static UINT __stdcall ThreadRecordControl(LPVOID pParameter);
	////////////////////////////////////////////////////////////////////////////////

protected: 
	DECLARE_DYNAMIC(CMainFrame)

	LRESULT  OnVideoWinMsg(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	
	LRESULT  OnStartStream(WPARAM wParam, LPARAM lParam);
	LRESULT  OnStopStream(WPARAM wParam, LPARAM lParam);

	LRESULT StartRec(WPARAM wParam, LPARAM lParam);
	LRESULT StopRec(WPARAM wParam, LPARAM lParam);

	 void OnTimer(UINT nIDEvent);
	 void OnDestroy();

	 void SelDevice();
	 void AutoRec();

	 void OnStatisticsView();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CVideoDisplayWnd    m_wndView;

	POINT              m_ptMenuClickPoint;

protected:
	RECT              m_paintRect;
	CRect             m_rcWin;
	CRect             m_rcVideo;


	UINT              m_frmCount;
    UINT              m_nFPS;

	HANDLE            m_fp;
	BOOL              m_bPreview;

	CString           m_strAppTitle;
	
	int		ntimer,autotimer,timertst;
public:
	CAVInputStream    m_InputStream;
	CAVOutputStream   m_OutputStream;
    CImagePainter     m_Painter;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickMenu(UINT id);
	afx_msg void OnUpdateMenuUI(CCmdUI* pCmdUI);

};


