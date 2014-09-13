
// SceneEditorView.h : interface of the CSceneEditorView class
//

#pragma once
#include "scenemanagerdlg.h"


class CSceneEditorView : public CView
{
protected: // create from serialization only
	CSceneEditorView();
	DECLARE_DYNCREATE(CSceneEditorView)

// Attributes
public:
	CSceneEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSceneEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	bool m_First;
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	void EngineSetup(void);
	afx_msg void OnEditSceneManager();
	CSceneManagerDlg *m_SceneManagerDlg;
	Ogre::SceneManager *m_SceneManager;
	Ogre::Root *m_Root;
	afx_msg void OnEditsceneAddscenenode();
	afx_msg void OnEditsceneAddcamera();
	afx_msg void OnEditsceneAddlight();
	afx_msg void OnEditsceneAddparticlesystem();
	afx_msg void OnEditsceneAddentity();
	afx_msg void OnEditsceneDestroy();
	afx_msg void OnEditsceneDestroyentities();
};

#ifndef _DEBUG  // debug version in SceneEditorView.cpp
inline CSceneEditorDoc* CSceneEditorView::GetDocument() const
   { return reinterpret_cast<CSceneEditorDoc*>(m_pDocument); }
#endif

