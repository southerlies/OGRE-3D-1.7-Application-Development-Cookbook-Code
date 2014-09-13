
// LightsView.h : interface of the CLightsView class
//

#pragma once
#include "LightsControlDlg.h"

class CLightsView : public CView
{
protected: // create from serialization only
	CLightsView();
	DECLARE_DYNCREATE(CLightsView)

// Attributes
public:
	CLightsDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLightsView();
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
   
		Ogre::Camera*			m_Camera;
		Ogre::RenderWindow*		m_RenderWindow;
		Ogre::SceneManager *SceneManager;
		CLightsControlDlg *m_LightsControlDlg;
		
		afx_msg void OnLightsControlLightsControl();
		afx_msg void OnPaint();
		void EngineSetup(void);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnLightscontrol1();
		afx_msg void OnLightscontrol2();
		afx_msg void OnLightscontrol3();
		afx_msg void OnLightscontrolTurnon();
		afx_msg void OnLightscontrolTurnoff();
};

#ifndef _DEBUG  // debug version in LightsView.cpp
inline CLightsDoc* CLightsView::GetDocument() const
   { return reinterpret_cast<CLightsDoc*>(m_pDocument); }
#endif

