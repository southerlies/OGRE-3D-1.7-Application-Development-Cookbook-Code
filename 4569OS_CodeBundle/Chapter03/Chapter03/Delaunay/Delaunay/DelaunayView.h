
// DelaunayView.h : interface of the CDelaunayView class
//

#pragma once
#include "triangulation.h"


class CDelaunayView : public CView
{
protected: // create from serialization only
	CDelaunayView();
	DECLARE_DYNCREATE(CDelaunayView)

// Attributes
public:
	CDelaunayDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CDelaunayView();
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
	
	void EngineSetup(void);
	afx_msg void OnPaint();
	void DelaunayIt(void);
	CTriangulation m_Triangulation;
	Ogre::ManualObject *m_Topography;
	bool m_MouseNavigation;
	CPoint m_MousePosition;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in DelaunayView.cpp
inline CDelaunayDoc* CDelaunayView::GetDocument() const
   { return reinterpret_cast<CDelaunayDoc*>(m_pDocument); }
#endif

