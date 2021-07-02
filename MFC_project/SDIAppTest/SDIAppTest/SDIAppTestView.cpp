
// SDIAppTestView.cpp : CSDIAppTestView 类的实现
//

#include "stdafx.h"
#include "stdio.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSDIAppTestView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSDIAppTestView 构造/析构

CSDIAppTestView::CSDIAppTestView()
{
	// TODO:  在此处添加构造代码
	printf("CSDIAppTestView::CSDIAppTestView() enter.\n");
	InitRect();
	InitPen();
}

CSDIAppTestView::~CSDIAppTestView()
{
}

BOOL CSDIAppTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSDIAppTestView 绘制

void CSDIAppTestView::OnDraw(CDC* /*pDC*/)
{
	CSDIAppTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	printf("CSDIAppTestView::OnDraw().\n");

	//采用GDI绘图
	CClientDC cdc(this);
	CPen* pOldPen = NULL;
	for (int i = 0; i<1; i++)
	{
		//把当前的画笔选入DC
		pOldPen = cdc.SelectObject(&m_myEclipse[i].pen);
		cdc.Ellipse(m_myEclipse[i].rect);
		cdc.SelectObject(pOldPen);
	}
}

void CSDIAppTestView::InitRect()
{
    //界面上要显示8个图形，准备好8个区域，分两行显示，每一行显示四个
    //图形左右间隔10个像素，上下间隔20个像素
    //左上 右下
    CRect rect(0,0,100,100);
    for (int i=0;i<8;i++)
    {
        if (i<4)
        {
            //第一行图形
            CRect rect;
            rect.left=20+i*150;
            rect.top=20;
            rect.right=100+i*150;
            rect.bottom=120;

            m_myEclipse[i].rect.CopyRect(rect);
        }
        else
        {
            //第二行图形
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
	//创建普通画笔
	m_myEclipse[0].pen.CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
	m_myEclipse[1].pen.CreatePen(PS_DASH, 5, RGB(0, 255, 0));
	m_myEclipse[2].pen.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	m_myEclipse[3].pen.CreatePen(PS_DASHDOT, 10, RGB(0, 0, 0));
	m_myEclipse[4].pen.CreatePen(PS_DASHDOTDOT, 5, RGB(255, 0, 255));
	m_myEclipse[5].pen.CreatePen(PS_NULL, 1, RGB(0, 255, 255));
	m_myEclipse[6].pen.CreatePen(PS_INSIDEFRAME, 10, RGB(0, 255, 255));

	//创建集合画笔
	LOGBRUSH LogBrush;
	LogBrush.lbStyle = BS_HATCHED;
	LogBrush.lbColor = RGB(0, 0, 255);
	LogBrush.lbHatch = HS_DIAGCROSS;
	m_myEclipse[7].pen.CreatePen(PS_GEOMETRIC, 20, &LogBrush);
}


// CSDIAppTestView 打印


void CSDIAppTestView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSDIAppTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSDIAppTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CSDIAppTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
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


// CSDIAppTestView 诊断

#ifdef _DEBUG
void CSDIAppTestView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIAppTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIAppTestDoc* CSDIAppTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIAppTestDoc)));
	return (CSDIAppTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CSDIAppTestView 消息处理程序
