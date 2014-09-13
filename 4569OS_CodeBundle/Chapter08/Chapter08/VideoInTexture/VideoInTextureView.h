
// VideoInTextureView.h : interface of the CVideoInTextureView class
//

#pragma once
#include <windows.h>
#include <Vfw.h>
#include "VideoTexture.h"

class CVideoInTextureView : public CView
{
protected: // create from serialization only
	CVideoInTextureView();
	DECLARE_DYNCREATE(CVideoInTextureView)

// Attributes
public:
	CVideoInTextureDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CVideoInTextureView();
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
   	bool m_MouseNavigation;
	CPoint m_MousePosition;
	Ogre::Camera* m_Camera;
    Ogre::RenderWindow*	m_RenderWindow;
	Ogre::SceneManager *m_SceneManager;
	CVideoTexture *m_VideoTexture;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	void EngineSetup(void);
	afx_msg void OnVideoStart();
	afx_msg void OnVideoStop();
};

#ifndef _DEBUG  // debug version in VideoInTextureView.cpp
inline CVideoInTextureDoc* CVideoInTextureView::GetDocument() const
   { return reinterpret_cast<CVideoInTextureDoc*>(m_pDocument); }
#endif

