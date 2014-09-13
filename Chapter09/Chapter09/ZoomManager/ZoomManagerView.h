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

// ZoomManagerView.h : interface of the CZoomManagerView class
//

#pragma once
#include "ZoomView.h"


class CZoomManagerView : public CView
{
protected: // create from serialization only
	CZoomManagerView();
	DECLARE_DYNCREATE(CZoomManagerView)

// Attributes
public:
	CZoomManagerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CZoomManagerView();
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
 	bool m_SelectMode;
	
	CRect m_rubberBand;
	CPoint m_Start;

//	CDC*		m_pdcMemory;
//	CBitmap*	m_pBitmap;
	float		m_zoomFactor;

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
	int SetMapMode(CDC* pDC);

	afx_msg void OnZoom();
	afx_msg void OnExtents();
	afx_msg void OnWindow();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in ZoomManagerView.cpp
inline CZoomManagerDoc* CZoomManagerView::GetDocument() const
   { return reinterpret_cast<CZoomManagerDoc*>(m_pDocument); }
#endif

