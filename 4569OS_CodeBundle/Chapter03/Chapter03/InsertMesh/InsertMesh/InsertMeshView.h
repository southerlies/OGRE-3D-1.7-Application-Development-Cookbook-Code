
// InsertMeshView.h : interface of the CInsertMeshView class
//

#pragma once
#include "triangulation.h"

class CInsertMeshView : public CView
{
protected: // create from serialization only
	CInsertMeshView();
	DECLARE_DYNCREATE(CInsertMeshView)

// Attributes
public:
	CInsertMeshDoc* GetDocument() const;

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
	virtual ~CInsertMeshView();
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
	
	void DelaunayIt(void);
	
	CTriangulation m_Triangulation;
	Ogre::ManualObject *m_Topography;
	bool m_MouseNavigation;
	CPoint m_MousePosition;

	void EngineSetup(void);
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // debug version in InsertMeshView.cpp
inline CInsertMeshDoc* CInsertMeshView::GetDocument() const
   { return reinterpret_cast<CInsertMeshDoc*>(m_pDocument); }
#endif

