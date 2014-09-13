
// SpeechView.h : interface of the CSpeechView class
//

#pragma once


class CSpeechView : public CView
{
protected: // create from serialization only
	CSpeechView();
	DECLARE_DYNCREATE(CSpeechView)

// Attributes
public:
	CSpeechDoc* GetDocument() const;

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
	virtual ~CSpeechView();
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
	Ogre::AnimationState    *m_AnimationState;
	Ogre::AnimationStateSet *m_AnimationStateSet;
	Ogre::Entity *m_HeadEntity;
	void EngineSetup(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in SpeechView.cpp
inline CSpeechDoc* CSpeechView::GetDocument() const
   { return reinterpret_cast<CSpeechDoc*>(m_pDocument); }
#endif

