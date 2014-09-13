
// MouseInputView.h : interface of the CMouseInputView class
//

#pragma once


class CMouseInputView : public CView
{
protected: // create from serialization only
	CMouseInputView();
	DECLARE_DYNCREATE(CMouseInputView)

// Attributes
public:
	CMouseInputDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMouseInputView();
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
	Ogre::AnimationState    *m_AnimationState;
	Ogre::AnimationStateSet *m_AnimationStateSet;
	Ogre::Entity *m_HeadEntity;
	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	bool m_MouseNavigation;
};

#ifndef _DEBUG  // debug version in MouseInputView.cpp
inline CMouseInputDoc* CMouseInputView::GetDocument() const
   { return reinterpret_cast<CMouseInputDoc*>(m_pDocument); }
#endif

