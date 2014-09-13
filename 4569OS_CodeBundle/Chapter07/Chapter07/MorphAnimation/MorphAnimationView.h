
// MorphAnimationView.h : interface of the CMorphAnimationView class
//

#pragma once


class CMorphAnimationView : public CView
{
protected: // create from serialization only
	CMorphAnimationView();
	DECLARE_DYNCREATE(CMorphAnimationView)

// Attributes
public:
	CMorphAnimationDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMorphAnimationView();
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
	Ogre::AnimationState *m_Animation;

	std::vector<Ogre::AnimationState*> m_AnimStateList;

        void EngineSetup(void);
		afx_msg void OnAnimationStart();
		afx_msg void OnAnimationStop();
};

#ifndef _DEBUG  // debug version in MorphAnimationView.cpp
inline CMorphAnimationDoc* CMorphAnimationView::GetDocument() const
   { return reinterpret_cast<CMorphAnimationDoc*>(m_pDocument); }
#endif

