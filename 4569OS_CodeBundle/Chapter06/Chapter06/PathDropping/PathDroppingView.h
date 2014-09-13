
// PathDroppingView.h : interface of the CPathDroppingView class
//

#pragma once
#include "Ogre.h"
#include "CollisionTools.h"

class CPathDroppingView : public CView
{
protected: // create from serialization only
	CPathDroppingView();
	DECLARE_DYNCREATE(CPathDroppingView)

// Attributes
public:
	CPathDroppingDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CPathDroppingView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DropPath(void);
void EngineSetup(void);
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	bool m_MouseNavigation;
	bool m_First;
	CPoint m_MousePosition;
	Ogre::AnimationState *m_Animation;		
	Ogre::Real m_RelativeDistance;
	MOC::CollisionTools* m_CollisionTools;
	CArray<Ogre::Vector3, Ogre::Vector3> m_Path;
	int m_PathIndex;
	afx_msg void OnTerrainVisible();
	afx_msg void OnTerrainUnvisible();
	afx_msg void OnWalkingStart();
	afx_msg void OnWalkingStop();
	afx_msg void OnWalkingDrap();
};

#ifndef _DEBUG  // debug version in PathDroppingView.cpp
inline CPathDroppingDoc* CPathDroppingView::GetDocument() const
   { return reinterpret_cast<CPathDroppingDoc*>(m_pDocument); }
#endif

