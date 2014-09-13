
// ResourceManagementView.h : interface of the CResourceManagementView class
//

#pragma once


class CResourceManagementView : public CView
{
protected: // create from serialization only
	CResourceManagementView();
	DECLARE_DYNCREATE(CResourceManagementView)

// Attributes
public:
	CResourceManagementDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CResourceManagementView();
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
};

#ifndef _DEBUG  // debug version in ResourceManagementView.cpp
inline CResourceManagementDoc* CResourceManagementView::GetDocument() const
   { return reinterpret_cast<CResourceManagementDoc*>(m_pDocument); }
#endif

