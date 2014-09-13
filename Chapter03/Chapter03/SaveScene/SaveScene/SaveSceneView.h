
// SaveSceneView.h : interface of the CSaveSceneView class
//

#pragma once
#include <xmlwriter.h>


class CSaveSceneView : public CView
{
protected: // create from serialization only
	CSaveSceneView();
	DECLARE_DYNCREATE(CSaveSceneView)

// Attributes
public:
	CSaveSceneDoc* GetDocument() const;

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
	virtual ~CSaveSceneView();
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
    xmlTextWriterPtr m_XmlWriter;

	void EngineSetup(void);
	afx_msg void OnPaint();
	void SceneExplore(Ogre::SceneManager * SceneManager);
	void SceneNodeExplore(Ogre::SceneNode * SceneNode);
};

#ifndef _DEBUG  // debug version in SaveSceneView.cpp
inline CSaveSceneDoc* CSaveSceneView::GetDocument() const
   { return reinterpret_cast<CSaveSceneDoc*>(m_pDocument); }
#endif

