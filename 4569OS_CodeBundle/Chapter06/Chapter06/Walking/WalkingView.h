
// WalkingView.h : interface of the CWalkingView class
//

#pragma once


class CWalkingView : public CView
{
protected: // create from serialization only
	CWalkingView();
	DECLARE_DYNCREATE(CWalkingView)

// Attributes
public:
	CWalkingDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CWalkingView();
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
	Ogre::AnimationState *m_Animation;		
	Ogre::Real m_Offset;
	Ogre::Real m_Angle;

	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

bool m_MouseNavigation;
afx_msg void OnWalkingStart();
afx_msg void OnWalkingStop();
};

#ifndef _DEBUG  // debug version in WalkingView.cpp
inline CWalkingDoc* CWalkingView::GetDocument() const
   { return reinterpret_cast<CWalkingDoc*>(m_pDocument); }
#endif

