
// SDIAppTestView.h : CSDIAppTestView 类的接口
//

#pragma once


class CSDIAppTestView : public CView
{
protected: // 仅从序列化创建
	CSDIAppTestView();
	DECLARE_DYNCREATE(CSDIAppTestView)

// 特性
public:
	CSDIAppTestDoc* GetDocument() const;

// 操作
public:
	struct my_Eclipse{
		CRect rect;  //图形显示区域
		CPen pen;  //画笔
	};
	//定义8个结构，用于绘制8个图形
	my_Eclipse  m_myEclipse[8];
	void InitRect();
	void InitPen();

public:
// 重写
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSDIAppTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SDIAppTestView.cpp 中的调试版本
inline CSDIAppTestDoc* CSDIAppTestView::GetDocument() const
   { return reinterpret_cast<CSDIAppTestDoc*>(m_pDocument); }
#endif

