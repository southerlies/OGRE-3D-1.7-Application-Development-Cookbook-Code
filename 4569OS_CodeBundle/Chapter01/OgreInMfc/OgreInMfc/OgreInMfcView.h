
// OgreInMfcView.h : interface of the COgreInMfcView class
//

#pragma once


class COgreInMfcView : public CView
{
protected: // create from serialization only
	COgreInMfcView();
	DECLARE_DYNCREATE(COgreInMfcView)

// Attributes
public:
	COgreInMfcDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~COgreInMfcView();
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
   
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	
	void EngineSetup(void);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

#ifndef _DEBUG  // debug version in OgreInMfcView.cpp
inline COgreInMfcDoc* COgreInMfcView::GetDocument() const
   { return reinterpret_cast<COgreInMfcDoc*>(m_pDocument); }
#endif

