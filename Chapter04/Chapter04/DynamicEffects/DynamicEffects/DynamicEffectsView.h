
// DynamicEffectsView.h : interface of the CDynamicEffectsView class
//

#pragma once
#include "DynamicEffectsDoc.h"
#include "DynamicEffectsControlDlg.h"

class CDynamicEffectsView : public CView
{
protected: // create from serialization only
	CDynamicEffectsView();
	DECLARE_DYNCREATE(CDynamicEffectsView)

// Attributes
public:
	CDynamicEffectsDoc* GetDocument() const;

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
	virtual ~CDynamicEffectsView();
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

	CPoint m_MousePosition;
	bool m_MouseNavigation;
	int m_WorkingTimer;
	bool m_First;
   	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	Ogre::Light *Light; 
	Ogre::ManualObject * Cone;
	CDynamicEffectsControlDlg *m_DynamicEffectsControlDlg;
	afx_msg void OnEffectsControlEffectsControl();
	afx_msg void OnPaint();
	void EngineSetup(void);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in DynamicEffectsView.cpp
inline CDynamicEffectsDoc* CDynamicEffectsView::GetDocument() const
   { return reinterpret_cast<CDynamicEffectsDoc*>(m_pDocument); }
#endif

