
// LandXmlView.h : interface of the CLandXmlView class
//

#pragma once


class CLandXmlView : public CView
{
protected: // create from serialization only
	CLandXmlView();
	DECLARE_DYNCREATE(CLandXmlView)

// Attributes
public:
	CLandXmlDoc* GetDocument() const;

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
	virtual ~CLandXmlView();
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
		CPoint m_MousePosition;
		bool m_MouseNavigation;
		Ogre::Camera*			m_Camera;
		Ogre::RenderWindow*		m_RenderWindow;
		Ogre::Entity *m_HeadEntity;
        void EngineSetup(void);
		Ogre::ManualObject * GetManualObject(char* LandXmlPath, char* SurfaceName);
		afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in LandXmlView.cpp
inline CLandXmlDoc* CLandXmlView::GetDocument() const
   { return reinterpret_cast<CLandXmlDoc*>(m_pDocument); }
#endif

