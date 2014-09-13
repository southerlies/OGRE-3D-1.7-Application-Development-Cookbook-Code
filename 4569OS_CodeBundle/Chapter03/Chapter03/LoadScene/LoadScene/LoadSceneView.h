
// LoadSceneView.h : interface of the CLoadSceneView class
//

#pragma once
#include <parser.h>
#include <tree.h>

class CLoadSceneView : public CView
{
protected: // create from serialization only
	CLoadSceneView();
	DECLARE_DYNCREATE(CLoadSceneView)

// Attributes
public:
	CLoadSceneDoc* GetDocument() const;

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
	virtual ~CLoadSceneView();
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
	Ogre::SceneManager *m_SceneManager;
	
	void EngineSetup(void);
	afx_msg void OnPaint();
	void Traverse(xmlNode * XmlNode, Ogre::SceneNode * SceneNode);
};

#ifndef _DEBUG  // debug version in LoadSceneView.cpp
inline CLoadSceneDoc* CLoadSceneView::GetDocument() const
   { return reinterpret_cast<CLoadSceneDoc*>(m_pDocument); }
#endif

