// SelectCaptureDeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "SelectCaptureDeviceDlg.h"

#include <objbase.h>
#include <uuids.h>
#include <strmif.h>
#include <devguid.h>
#include <regstr.h>
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "Strmiids.lib")

extern int selflag;
extern char camdes[],micdes[];
extern char tmps[],cfgwjm[],rundir[],tmpwjm[],jgpath[];
extern CString pszFileName,psztmp;
extern int nIdx,index;

// CSelectCaptureDeviceDlg 对话框

IMPLEMENT_DYNAMIC(CSelectCaptureDeviceDlg, CDialog)

CSelectCaptureDeviceDlg::CSelectCaptureDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectCaptureDeviceDlg::IDD, pParent)
	, m_jgdir(_T(""))
{

}

CSelectCaptureDeviceDlg::~CSelectCaptureDeviceDlg()
{
}

void CSelectCaptureDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_jgdir);
}


BEGIN_MESSAGE_MAP(CSelectCaptureDeviceDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectCaptureDeviceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_VIDEODEV, &CSelectCaptureDeviceDlg::OnCheckEnableVideodev)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_AUDIODEV, &CSelectCaptureDeviceDlg::OnCheckEnableAudiodev)
	ON_BN_CLICKED(IDC_BTNSAVE, &CSelectCaptureDeviceDlg::OnBnClickedBtnsave)
	ON_BN_CLICKED(IDC_BUTTON2, &CSelectCaptureDeviceDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

//Show list of devices  
void CSelectCaptureDeviceDlg::show_dshow_device()
{
#if 0
    AVFormatContext *pFmtCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options, "list_devices", "true", 0);
    AVInputFormat *iformat = av_find_input_format("dshow");
    //printf("Device Info=============\n");
    avformat_open_input(&pFmtCtx, "video=dummy", iformat, &options);
    //printf("========================\n");
#endif
	CComboBox *pbox;

	::CoInitialize(NULL); //调用DirectShow SDK的API需要用到COM库


    int iVideoCapDevNum = 0;
	int iAudioCapDevNum = 0;

	pbox=(CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES);
	DS_GetAudioVideoInputDevices(pbox,CLSID_VideoInputDeviceCategory);		////视频输入设备
	iVideoCapDevNum=pbox->GetCount();

	pbox=(CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES);
	DS_GetAudioVideoInputDevices(pbox,CLSID_AudioInputDeviceCategory);		////音频输入设备
	iAudioCapDevNum=pbox->GetCount();

	if(iVideoCapDevNum > 0)
	{
	  ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_VIDEODEV))->SetCheck(TRUE);
	}
	
	if(iAudioCapDevNum > 0)
	{
	   ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_AUDIODEV))->SetCheck(TRUE);
	}

	CoUninitialize();

}


HRESULT CSelectCaptureDeviceDlg::DS_GetAudioVideoInputDevices( CComboBox *m_box, GUID guidValue )
{
	//	TDeviceName name; 
	HRESULT hr;

	// 初始化COM
	hr = ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
	if (FAILED(hr))
	{
		return hr;
	}

	// 创建系统设备枚举器实例
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance( CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum );
	if (FAILED(hr))
	{
		::CoUninitialize();
		return hr;
	}

	// 获取设备类枚举器		
	IEnumMoniker *pEnumCat = NULL;
	//	hr = pSysDevEnum->CreateClassEnumerator( CLSID_AudioInputDeviceCategory, &pEnumCat, 0 );	////音频输入设备
	//	hr = pSysDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEnumCat, 0 );	////视频输入设备
	hr = pSysDevEnum->CreateClassEnumerator( guidValue, &pEnumCat, 0 );	////输入设备
	if (hr == S_OK) 
	{
		// 枚举设备名称
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		while(pEnumCat->Next( 1, &pMoniker, &cFetched ) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage( NULL, NULL, IID_IPropertyBag, (void **)&pPropBag );
			if (SUCCEEDED(hr))
			{
				// 获取设备友好名
				VARIANT varName;
				VariantInit( &varName );

				hr = pPropBag->Read( L"FriendlyName", &varName, NULL );
				if (SUCCEEDED(hr))
				{
					CString Recstring;
					Recstring.Empty();
					Recstring = varName.bstrVal;

					m_box->AddString(Recstring);
				}

				VariantClear( &varName );
				pPropBag->Release();						
			}

			pMoniker->Release();
		} // End for While

		pEnumCat->Release();
	}

	pSysDevEnum->Release();
	CoUninitialize();

	return hr;
}

// CSelectCaptureDeviceDlg 消息处理程序
BOOL CSelectCaptureDeviceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	show_dshow_device();

	m_jgdir=jgpath;
	UpdateData(FALSE);

	CComboBox *pbox;
	int inx,maxnum,inxaudio,inxvideo;
	char xstr[300];

	inxaudio=inxvideo=-1;

	pbox=(CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES);
	maxnum=pbox->GetCount();
	if(maxnum>0) pbox->SetCurSel(0);
	for(inx=0;(inx<maxnum) && (strlen(camdes)>1) ;inx++)
	{
		pbox->GetLBText(inx,xstr);
		if(strstr(xstr,camdes)!=NULL)
		{
			pbox->SetCurSel(inx);
			inxvideo=inx;
			break;
		}
	}
	pbox=(CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES);
	maxnum=pbox->GetCount();
	if(maxnum>0) pbox->SetCurSel(0);
	for(inx=0;(inx<maxnum) && (strlen(micdes)>1) ;inx++)
	{
		pbox->GetLBText(inx,xstr);
		if(strstr(xstr,micdes)!=NULL)
		{
			pbox->SetCurSel(inx);
			inxaudio=inx;
			break;
		}
	}
	////////////////////////////////////////////////
	if(inxaudio<0)
	{
		for(inx=0;inx<maxnum;inx++)
		{
			pbox->GetLBText(inx,xstr);
			if(strstr(xstr,"麦克风")!=NULL)
			{
				pbox->SetCurSel(inx);
				inxaudio=inx;
				break;
			}
		}
	}
	if(inxaudio<0)
	{
		for(inx=0;inx<maxnum;inx++)
		{
			pbox->GetLBText(inx,xstr);
			if(strstr(xstr,"microphone")!=NULL)
			{
				pbox->SetCurSel(inx);
				inxaudio=inx;
				break;
			}
		}
	}
	if(inxaudio<0)
	{
		for(inx=0;inx<maxnum;inx++)
		{
			pbox->GetLBText(inx,xstr);
			if(strstr(xstr,"线路音量")!=NULL)
			{
				pbox->SetCurSel(inx);
				inxaudio=inx;
				break;
			}
		}
	}
	if(inxaudio<0)
	{
		for(inx=0;inx<maxnum;inx++)
		{
			pbox->GetLBText(inx,xstr);
			if(strstr(xstr,"LineIn")!=NULL)
			{
				pbox->SetCurSel(inx);
				inxaudio=inx;
				break;
			}
		}
	}
	if( (inxvideo<0) || (inxaudio<0) )
	{
		selflag=0;
	}

	if(selflag>1)
	{
		OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CSelectCaptureDeviceDlg::OnCheckEnableVideodev()
{
	BOOL bCapVideo = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_VIDEODEV))->GetCheck();

	((CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES))->EnableWindow(bCapVideo ? TRUE : FALSE);
}

void CSelectCaptureDeviceDlg::OnCheckEnableAudiodev()
{
	BOOL bCapAudio = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_AUDIODEV))->GetCheck();

	((CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES))->EnableWindow(bCapAudio ? TRUE : FALSE);
}

void CSelectCaptureDeviceDlg::OnBnClickedOk()
{
	BOOL bCapVideo = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_VIDEODEV))->GetCheck();
	BOOL bCapAudio = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_AUDIODEV))->GetCheck();

	if(bCapVideo)
	{
		int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES))->GetCurSel();
		if(nCurSel >= 0)
		{
		   ((CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES))->GetLBText(nCurSel, camdes);
		}
	}

	if(bCapAudio)
	{
		int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES))->GetCurSel();
		if(nCurSel >= 0)
		{
		   ((CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES))->GetLBText(nCurSel, micdes);
		}
	}

	if( (strlen(camdes)<1) && (strlen(micdes)<1) )
	{
		MessageBox(_T("你没有选择任何设备"), _T("提示"), MB_OK|MB_ICONWARNING);
		return;
	}

	OnOK();
}

void CSelectCaptureDeviceDlg::OnBnClickedBtnsave()
{
	camdes[0]='\0';
	micdes[0]='\0';

	strcpy(jgpath,m_jgdir);

	psztmp.Format("\\%d.",nIdx);
	pszFileName = jgpath+psztmp;

	psztmp=pszFileName;
	pszFileName=psztmp+"mkv";

	if(GetFileAttributes(pszFileName)!=-1) DeleteFile(pszFileName);

	int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES))->GetCurSel();
	if(nCurSel >= 0)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_VIDEO_DEVICES))->GetLBText(nCurSel, camdes);
	}

	nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES))->GetCurSel();
	if(nCurSel >= 0)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_DEVICES))->GetLBText(nCurSel, micdes);
	}

	if( (strlen(camdes)>2) && (strlen(micdes)>2) )
	{
		////////////写入信息/////////
		WritePrivateProfileString("录像回放配置","录像摄像头名称",camdes,cfgwjm);
		WritePrivateProfileString("录像回放配置","录音通道名称",micdes,cfgwjm);
		WritePrivateProfileString("录像回放配置","视频文件保存路径",jgpath,cfgwjm);
	}
}

void CSelectCaptureDeviceDlg::OnBnClickedButton2()
{
	CString strResult =""; 
	LPMALLOC lpMalloc;  // pointer to IMalloc 
	if (::SHGetMalloc(&lpMalloc) != NOERROR) 
	{ 
		AfxMessageBox("路径浏览出错!"); 
		return ; 
	} 
	char szDisplayName[_MAX_PATH]; 
	char szBuffer[_MAX_PATH]; 
	BROWSEINFO browseInfo; 

	szBuffer[0] = '\0';
	browseInfo.hwndOwner = this->m_hWnd; 
	// set root at Desktop 
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szDisplayName; 
	browseInfo.lpszTitle = "请选择视频文件的保存路径";  // Dialog title 
	browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS; 
	browseInfo.lpfn = NULL;      // not used 
	browseInfo.lParam = 0;      // not used 
	LPITEMIDLIST lpItemIDList; 
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL) 
	{ 
		// Get the path of the selected folder from the    item ID list. 
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer)) 
		{ 
			// At this point, szBuffer contains the path the user chose. 
			if (szBuffer[0] == '\0') 
			{ 
				// SHGetPathFromIDList failed, or SHBrowseForFolder failed. 
				AfxMessageBox("获取路径错误!",MB_ICONSTOP|MB_OK); 
				return ; 
			} 
			// We have a path in szBuffer! Return it. 
			strResult = szBuffer; 
			if(strResult.GetLength()>1) m_jgdir=strResult;
		} 
		else 
		{ 
			// The thing referred to by lpItemIDList 
			// might not have been a file system object. 
			// For whatever reason, SHGetPathFromIDList didn't work! 
			AfxMessageBox("获取路径错误!", MB_ICONSTOP|MB_OK); 
			return ; 
		} 
		lpMalloc->Free(lpItemIDList); 
		lpMalloc->Release(); 
	} 

	UpdateData(FALSE); 	
}
