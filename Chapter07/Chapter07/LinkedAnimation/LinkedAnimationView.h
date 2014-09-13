
// LinkedAnimationView.h : interface of the CLinkedAnimationView class
//

#pragma once


class CLinkedAnimationView : public CView
{
protected: // create from serialization only
	CLinkedAnimationView();
	DECLARE_DYNCREATE(CLinkedAnimationView)

// Attributes
public:
	CLinkedAnimationDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CLinkedAnimationView();
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
public:
	bool m_First;
	bool m_MouseNavigation;
	CPoint m_MousePosition;
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	Ogre::AnimationState *m_AnimationState1;
	Ogre::AnimationState *m_AnimationState2;
	Ogre::Animation *m_Animation;

        void EngineSetup(void);
		afx_msg void OnAnimationStart();
		afx_msg void OnAnimationStop();
};

#ifndef _DEBUG  // debug version in LinkedAnimationView.cpp
inline CLinkedAnimationDoc* CLinkedAnimationView::GetDocument() const
   { return reinterpret_cast<CLinkedAnimationDoc*>(m_pDocument); }
#endif

