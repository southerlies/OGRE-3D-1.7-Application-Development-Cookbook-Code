
// LinkedControllersView.h : interface of the CLinkedControllersView class
//

#pragma once


class CLinkedControllersView : public CView
{
protected: // create from serialization only
	CLinkedControllersView();
	DECLARE_DYNCREATE(CLinkedControllersView)

// Attributes
public:
	CLinkedControllersDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CLinkedControllersView();
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

	Ogre::Controller<Ogre::Real>* m_RedController;
	Ogre::Controller<Ogre::Real>* m_BlueController;
	Ogre::Controller<Ogre::Real>* m_WhiteController;

 Ogre::BillboardSet* m_Lights;
 
 Ogre::Billboard* m_RedBillBoard;
 Ogre::Billboard* m_BlueBillBoard;
 Ogre::Billboard* m_WhiteBillBoard;
 
 Ogre::Light* m_RedLight;
 Ogre::Light* m_BlueLight;
 Ogre::Light* m_WhiteLight;

        void EngineSetup(void);
		afx_msg void OnAnimationStart();
		afx_msg void OnAnimationStop();
};

#ifndef _DEBUG  // debug version in LinkedControllersView.cpp
inline CLinkedControllersDoc* CLinkedControllersView::GetDocument() const
   { return reinterpret_cast<CLinkedControllersDoc*>(m_pDocument); }
#endif

