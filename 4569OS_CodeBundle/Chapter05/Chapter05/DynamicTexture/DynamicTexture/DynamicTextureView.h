
// DynamicTextureView.h : interface of the CDynamicTextureView class
//

#pragma once
#include <windows.h>
#include <Vfw.h>

class CDynamicTextureView : public CView
{
protected: // create from serialization only
	CDynamicTextureView();
	DECLARE_DYNCREATE(CDynamicTextureView)

// Attributes
public:
	CDynamicTextureDoc* GetDocument() const;

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
	virtual ~CDynamicTextureView();
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
	int m_Shift;
	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_FrameNumber;
	
	PGETFRAME  Frame;
	BITMAPFILEHEADER BMPFileHeader;
	PAVIFILE   aviFile;
	PAVISTREAM  aviStream;
	AVISTREAMINFO  aviStreamInfo;
	Ogre::HardwarePixelBufferSharedPtr PixelBuffer;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTvcontrolStart();
	afx_msg void OnTvcontrolStop();
	afx_msg void OnTvcontrolShift();
};

#ifndef _DEBUG  // debug version in DynamicTextureView.cpp
inline CDynamicTextureDoc* CDynamicTextureView::GetDocument() const
   { return reinterpret_cast<CDynamicTextureDoc*>(m_pDocument); }
#endif

