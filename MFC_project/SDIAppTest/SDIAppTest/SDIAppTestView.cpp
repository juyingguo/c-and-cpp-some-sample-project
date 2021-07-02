
// SDIAppTestView.cpp : CSDIAppTestView ���ʵ��
//

#include "stdafx.h"
#include "stdio.h"
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
	printf("CSDIAppTestView::CSDIAppTestView() enter.\n");
	InitRect();
	InitPen();
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
	printf("CSDIAppTestView::OnDraw().\n");

	//����GDI��ͼ
	CClientDC cdc(this);
	CPen* pOldPen = NULL;
	for (int i = 0; i<1; i++)
	{
		//�ѵ�ǰ�Ļ���ѡ��DC
		pOldPen = cdc.SelectObject(&m_myEclipse[i].pen);
		cdc.Ellipse(m_myEclipse[i].rect);
		cdc.SelectObject(pOldPen);
	}
}

void CSDIAppTestView::InitRect()
{
    //������Ҫ��ʾ8��ͼ�Σ�׼����8�����򣬷�������ʾ��ÿһ����ʾ�ĸ�
    //ͼ�����Ҽ��10�����أ����¼��20������
    //���� ����
    CRect rect(0,0,100,100);
    for (int i=0;i<8;i++)
    {
        if (i<4)
        {
            //��һ��ͼ��
            CRect rect;
            rect.left=20+i*150;
            rect.top=20;
            rect.right=100+i*150;
            rect.bottom=120;

            m_myEclipse[i].rect.CopyRect(rect);
        }
        else
        {
            //�ڶ���ͼ��
            CRect rect;
            rect.left=20+(i-4)*150;
            rect.top=250;
            rect.right=100+(i-4)*150;
            rect.bottom=350;

            m_myEclipse[i].rect.CopyRect(rect);
        }
       
    }
}
void CSDIAppTestView::InitPen()
{
	//������ͨ����
	m_myEclipse[0].pen.CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
	m_myEclipse[1].pen.CreatePen(PS_DASH, 5, RGB(0, 255, 0));
	m_myEclipse[2].pen.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	m_myEclipse[3].pen.CreatePen(PS_DASHDOT, 10, RGB(0, 0, 0));
	m_myEclipse[4].pen.CreatePen(PS_DASHDOTDOT, 5, RGB(255, 0, 255));
	m_myEclipse[5].pen.CreatePen(PS_NULL, 1, RGB(0, 255, 255));
	m_myEclipse[6].pen.CreatePen(PS_INSIDEFRAME, 10, RGB(0, 255, 255));

	//�������ϻ���
	LOGBRUSH LogBrush;
	LogBrush.lbStyle = BS_HATCHED;
	LogBrush.lbColor = RGB(0, 0, 255);
	LogBrush.lbHatch = HS_DIAGCROSS;
	m_myEclipse[7].pen.CreatePen(PS_GEOMETRIC, 20, &LogBrush);
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
