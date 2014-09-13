
// AddingTreesView.h : interface of the CAddingTreesView class
//

#pragma once


class CAddingTreesView : public CView
{
protected: // create from serialization only
	CAddingTreesView();
	DECLARE_DYNCREATE(CAddingTreesView)

// Attributes
public:
	CAddingTreesDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CAddingTreesView();
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
	bool m_First;
   
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	CPoint m_MousePosition;
	bool m_MouseNavigation;
	
	void EngineSetup(void);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in AddingTreesView.cpp
inline CAddingTreesDoc* CAddingTreesView::GetDocument() const
   { return reinterpret_cast<CAddingTreesDoc*>(m_pDocument); }
#endif

