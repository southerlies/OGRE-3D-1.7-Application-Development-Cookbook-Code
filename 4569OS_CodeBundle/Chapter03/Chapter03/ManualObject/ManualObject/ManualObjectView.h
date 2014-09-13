
// ManualObjectView.h : interface of the CManualObjectView class
//

#pragma once


class CManualObjectView : public CView
{
protected: // create from serialization only
	CManualObjectView();
	DECLARE_DYNCREATE(CManualObjectView)

// Attributes
public:
	CManualObjectDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CManualObjectView();
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
	Ogre::ManualObject* m_ManualObject;
	Ogre::SceneManager *m_SceneManager;
	int m_MaterialIndex;

	void EngineSetup(void);
	afx_msg void OnPaint();
	void CreateManualObject(void);
	afx_msg void OnActionsMaterials();
	afx_msg void OnActionsChangeoperations();
	afx_msg void OnActionsReset();
};

#ifndef _DEBUG  // debug version in ManualObjectView.cpp
inline CManualObjectDoc* CManualObjectView::GetDocument() const
   { return reinterpret_cast<CManualObjectDoc*>(m_pDocument); }
#endif

