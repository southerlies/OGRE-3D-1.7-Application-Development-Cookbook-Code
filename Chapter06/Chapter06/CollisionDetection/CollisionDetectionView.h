
// CollisionDetectionView.h : interface of the CCollisionDetectionView class
//

#pragma once
#include "CollisionTools.h"

class CCollisionDetectionView : public CView
{
protected: // create from serialization only
	CCollisionDetectionView();
	DECLARE_DYNCREATE(CCollisionDetectionView)

// Attributes
public:
	CCollisionDetectionDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CCollisionDetectionView();
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
	
	CPoint m_MousePosition;

	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
		
	MOC::CollisionTools* m_CollisionTools;

	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
bool m_MouseNavigation;
};

#ifndef _DEBUG  // debug version in CollisionDetectionView.cpp
inline CCollisionDetectionDoc* CCollisionDetectionView::GetDocument() const
   { return reinterpret_cast<CCollisionDetectionDoc*>(m_pDocument); }
#endif

