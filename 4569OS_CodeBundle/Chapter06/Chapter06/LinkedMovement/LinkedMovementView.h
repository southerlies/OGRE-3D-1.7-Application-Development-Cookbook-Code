
// LinkedMovementView.h : interface of the CLinkedMovementView class
//

#pragma once
#include "c:\users\ilya\documents\visual studio 2010\projects\recipes\ogresdk_vc10_v1-7-1\include\ogre\ogremath.h"
#include "c:\users\ilya\documents\visual studio 2010\projects\recipes\ogresdk_vc10_v1-7-1\include\ogre\ogremath.h"


class CLinkedMovementView : public CView
{
protected: // create from serialization only
	CLinkedMovementView();
	DECLARE_DYNCREATE(CLinkedMovementView)

// Attributes
public:
	CLinkedMovementDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CLinkedMovementView();
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

	bool m_First;
	
	CPoint m_MousePosition;
	bool m_MouseNavigation;
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	void EngineSetup(void);
	Ogre::Radian m_RotationAngle;
	Ogre::Radian m_AngleIncrement;
	//  

	Ogre::SceneNode *m_SystemNode;
	afx_msg void OnManagementStart();
	afx_msg void OnManagementStop();
};

#ifndef _DEBUG  // debug version in LinkedMovementView.cpp
inline CLinkedMovementDoc* CLinkedMovementView::GetDocument() const
   { return reinterpret_cast<CLinkedMovementDoc*>(m_pDocument); }
#endif

