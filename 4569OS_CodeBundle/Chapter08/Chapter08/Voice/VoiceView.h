
// VoiceView.h : interface of the CVoiceView class
//

#pragma once
#include <sphelper.h>

class CVoiceView : public CView
{
protected: // create from serialization only
	CVoiceView();
	DECLARE_DYNCREATE(CVoiceView)

// Attributes
public:
	CVoiceDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CVoiceView();
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
    Ogre::SceneManager *m_SceneManager;
    Ogre::RenderWindow*	m_RenderWindow;
	int m_Event;

	Ogre::AnimationState *m_WalkAnimation1;
	Ogre::AnimationState *m_WalkAnimation2;

	CComPtr<ISpVoice> m_Voice;
	
	CComPtr<ISpObjectToken> m_cpVoiceToken1;
	CComPtr<ISpObjectToken> m_cpVoiceToken2;

	CComPtr<IEnumSpObjectTokens> m_cpEnum1;
	CComPtr<IEnumSpObjectTokens> m_cpEnum2;

	void EngineSetup(void);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnAnimationStart();
	afx_msg void OnAnimationStop();
};

#ifndef _DEBUG  // debug version in VoiceView.cpp
inline CVoiceDoc* CVoiceView::GetDocument() const
   { return reinterpret_cast<CVoiceDoc*>(m_pDocument); }
#endif

