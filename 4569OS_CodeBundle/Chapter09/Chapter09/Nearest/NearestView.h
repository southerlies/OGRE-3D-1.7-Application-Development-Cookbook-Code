// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// NearestView.h : interface of the CNearestView class
//

#pragma once


class CNearestView : public CView
{
protected: // create from serialization only
	CNearestView();
	DECLARE_DYNCREATE(CNearestView)

// Attributes
public:
	CNearestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CNearestView();
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
   	bool m_MouseNavigation;
	bool m_SelectionMode;
	unsigned long  m_QueryMask;

	CPoint m_MousePosition;
	Ogre::Camera* m_Camera;
    Ogre::RenderWindow*	m_RenderWindow;
	Ogre::SceneManager *m_SceneManager;
	Ogre::AnimationState *m_WalkAnimation;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void EngineSetup(void);

	afx_msg void OnEntity();
	afx_msg void OnBillboard();
	afx_msg void OnManualObject();
	afx_msg void OnCamera();
	afx_msg void OnSelect();
};

#ifndef _DEBUG  // debug version in NearestView.cpp
inline CNearestDoc* CNearestView::GetDocument() const
   { return reinterpret_cast<CNearestDoc*>(m_pDocument); }
#endif

