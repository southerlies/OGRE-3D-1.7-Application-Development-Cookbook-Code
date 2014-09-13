
// PathWalkingView.h : interface of the CPathWalkingView class
//

#pragma once
#include "afxtempl.h"


class CPathWalkingView : public CView
{
protected: // create from serialization only
	CPathWalkingView();
	DECLARE_DYNCREATE(CPathWalkingView)

// Attributes
public:
	CPathWalkingDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CPathWalkingView();
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
	Ogre::Real m_Offset;
	Ogre::Real m_Angle;
	Ogre::Real m_RelativeDistance;

	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();

bool m_MouseNavigation;
afx_msg void OnWalkControl();
afx_msg void OnWalkStart();
afx_msg void OnWalkStop();
void CreatePath(void);
CArray<Ogre::Vector3, Ogre::Vector3> m_Path;
int m_PathIndex;
};

#ifndef _DEBUG  // debug version in PathWalkingView.cpp
inline CPathWalkingDoc* CPathWalkingView::GetDocument() const
   { return reinterpret_cast<CPathWalkingDoc*>(m_pDocument); }
#endif

