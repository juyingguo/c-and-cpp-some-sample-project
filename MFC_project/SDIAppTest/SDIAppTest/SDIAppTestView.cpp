
// SDIAppTestView.cpp : CSDIAppTestView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "SDIAppTest.h"
#endif

#include "SDIAppTestDoc.h"
#include "SDIAppTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIAppTestView

IMPLEMENT_DYNCREATE(CSDIAppTestView, CView)

BEGIN_MESSAGE_MAP(CSDIAppTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSDIAppTestView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSDIAppTestView ����/����

CSDIAppTestView::CSDIAppTestView()
{
	// TODO:  �ڴ˴���ӹ������

}

CSDIAppTestView::~CSDIAppTestView()
{
}

BOOL CSDIAppTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSDIAppTestView ����

void CSDIAppTestView::OnDraw(CDC* /*pDC*/)
{
	CSDIAppTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CSDIAppTestView ��ӡ


void CSDIAppTestView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSDIAppTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSDIAppTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSDIAppTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void CSDIAppTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSDIAppTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSDIAppTestView ���

#ifdef _DEBUG
void CSDIAppTestView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIAppTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIAppTestDoc* CSDIAppTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIAppTestDoc)));
	return (CSDIAppTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CSDIAppTestView ��Ϣ�������
