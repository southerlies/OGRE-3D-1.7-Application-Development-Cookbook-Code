
// VideoView.h : interface of the CVideoView class
//

#pragma once
#include "AviFile.h"

class CVideoView : public CView
{
protected: // create from serialization only
	CVideoView();
	DECLARE_DYNCREATE(CVideoView)

// Attributes
public:
	CVideoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CVideoView();
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
	//Ogre::PixelBox m_PixelBox;

	PGETFRAME  Frame;
	BITMAPFILEHEADER BMPFileHeader;
	PAVIFILE   aviFile;
	PAVISTREAM  aviStream;
	AVISTREAMINFO  aviStreamInfo;
	long m_FrameNumber;
	unsigned int mWidth;
    unsigned int mHeight;
	unsigned int mColorDepth;
	Ogre::uchar *data;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void EngineSetup(void);
	afx_msg void OnVideoStart();
	afx_msg void OnVideoStop();
	afx_msg void OnAnimationStart();
	afx_msg void OnAnimationStop();
};

#ifndef _DEBUG  // debug version in VideoView.cpp
inline CVideoDoc* CVideoView::GetDocument() const
   { return reinterpret_cast<CVideoDoc*>(m_pDocument); }
#endif

