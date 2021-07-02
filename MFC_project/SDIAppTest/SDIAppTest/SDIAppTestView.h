
// SDIAppTestView.h : CSDIAppTestView ��Ľӿ�
//

#pragma once


class CSDIAppTestView : public CView
{
protected: // �������л�����
	CSDIAppTestView();
	DECLARE_DYNCREATE(CSDIAppTestView)

// ����
public:
	CSDIAppTestDoc* GetDocument() const;

// ����
public:
	struct my_Eclipse{
		CRect rect;  //ͼ����ʾ����
		CPen pen;  //����
	};
	//����8���ṹ�����ڻ���8��ͼ��
	my_Eclipse  m_myEclipse[8];
	void InitRect();
	void InitPen();

public:
// ��д
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CSDIAppTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SDIAppTestView.cpp �еĵ��԰汾
inline CSDIAppTestDoc* CSDIAppTestView::GetDocument() const
   { return reinterpret_cast<CSDIAppTestDoc*>(m_pDocument); }
#endif

