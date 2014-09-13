
// ImageEditorView.h : interface of the CImageEditorView class
//

#pragma once


class CImageEditorView : public CView
{
protected: // create from serialization only
	CImageEditorView();
	DECLARE_DYNCREATE(CImageEditorView)

// Attributes
public:
	CImageEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CImageEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	bool m_First;
   
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	
	void EngineSetup(void);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in ImageEditorView.cpp
inline CImageEditorDoc* CImageEditorView::GetDocument() const
   { return reinterpret_cast<CImageEditorDoc*>(m_pDocument); }
#endif

