
// TerrainWalkingView.h : interface of the CTerrainWalkingView class
//

#pragma once
#include "CollisionTools.h"

class CTerrainWalkingView : public CView
{
protected: // create from serialization only
	CTerrainWalkingView();
	DECLARE_DYNCREATE(CTerrainWalkingView)

// Attributes
public:
	CTerrainWalkingDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTerrainWalkingView();
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
	
	CPoint m_MousePosition;

	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	Ogre::AnimationState *m_Animation;		
	Ogre::Real m_RelativeDistance;
	MOC::CollisionTools* m_CollisionTools;
	
	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();

bool m_MouseNavigation;
afx_msg void OnWalkingStart();
afx_msg void OnWalkingStop();
afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in TerrainWalkingView.cpp
inline CTerrainWalkingDoc* CTerrainWalkingView::GetDocument() const
   { return reinterpret_cast<CTerrainWalkingDoc*>(m_pDocument); }
#endif

