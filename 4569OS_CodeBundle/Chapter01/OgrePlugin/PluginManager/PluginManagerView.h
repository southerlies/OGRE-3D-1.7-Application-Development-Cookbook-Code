
// PluginManagerView.h : interface of the CPluginManagerView class
//

#pragma once


class CPluginManagerView : public CView
{
protected: // create from serialization only
	CPluginManagerView();
	DECLARE_DYNCREATE(CPluginManagerView)

// Attributes
public:
	CPluginManagerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CPluginManagerView();
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
	afx_msg void OnLoadRobot();
};

#ifndef _DEBUG  // debug version in PluginManagerView.cpp
inline CPluginManagerDoc* CPluginManagerView::GetDocument() const
   { return reinterpret_cast<CPluginManagerDoc*>(m_pDocument); }
#endif

