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

// SuperEllipsoidView.h : interface of the CSuperEllipsoidView class
//

#pragma once
#include "superquadric.h"


class CSuperEllipsoidView : public CView
{
protected: // create from serialization only
	CSuperEllipsoidView();
	DECLARE_DYNCREATE(CSuperEllipsoidView)

// Attributes
public:
	CSuperEllipsoidDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSuperEllipsoidView();
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
	afx_msg void OnHorizontal();
	afx_msg void OnVertical();

	bool m_First;
	CSuperQuadric m_SuperQuadric;
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	CPoint m_MousePosition;
	bool m_MouseNavigation;
	void EngineSetup(void);

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHorizontalEdit();
	afx_msg void OnVerticalEdit();
};

#ifndef _DEBUG  // debug version in SuperEllipsoidView.cpp
inline CSuperEllipsoidDoc* CSuperEllipsoidView::GetDocument() const
   { return reinterpret_cast<CSuperEllipsoidDoc*>(m_pDocument); }
#endif

