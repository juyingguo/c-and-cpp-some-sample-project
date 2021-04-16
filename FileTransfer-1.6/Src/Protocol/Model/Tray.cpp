/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : Tray.cpp
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.02.24
 * �ļ�˵�� : implementation of the CTray class.
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTray::CTray(HWND m_hWnd, int _uID)
{
	this->m_hWnd = m_hWnd;
	this->_uID = _uID;
}

CTray::~CTray()
{

}
/***************************************************************************/
/***********************������CTray������ṩ��ͳһ�ӿ�*********************/
/***************************************************************************/

/*---------------------------------------------------------------------------
 * �������� : ToTray
 * �������� : ��С��Ӧ�ó�������
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CTray::ToTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize				= (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd				= m_hWnd;
	nid.uID					= _uID;
	nid.uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage	= WM_SHOWTASK;						//�Զ������Ϣ����
	nid.hIcon				= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(_uID));
	strcpy(nid.szTip,"��������");								//��Ϣ��ʾ��
	Shell_NotifyIcon(NIM_ADD,&nid);								//�����������ͼ��
	::ShowWindow(m_hWnd, SW_HIDE);								//����������
}
/*---------------------------------------------------------------------------
 * �������� : DeleteTray
 * �������� : ɾ������ͼ��
 * ��ڲ��� : ��
 * ���ڲ��� : ��
 ---------------------------------------------------------------------------*/
void CTray::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize				= (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd				= m_hWnd;
	nid.uID					= _uID;
	nid.uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage	= WM_SHOWTASK;						//�Զ������Ϣ����
	nid.hIcon				= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(_uID));
	strcpy(nid.szTip,"��������");								//��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
	Shell_NotifyIcon(NIM_DELETE,&nid);							//��������ɾ��ͼ��
}
