
// DynamicGrammarView.h : interface of the CDynamicGrammarView class
//

#pragma once
#define WM_RECOEVENT WM_USER+1


class CDynamicGrammarView : public CView
{
protected: // create from serialization only
	CDynamicGrammarView();
	DECLARE_DYNCREATE(CDynamicGrammarView)

// Attributes
public:
	CDynamicGrammarDoc* GetDocument() const;

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
	virtual ~CDynamicGrammarView();
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
protected:
	afx_msg LRESULT OnRecoEvent(WPARAM wParam, LPARAM lParam);
public:
	Ogre::Camera*			m_Camera;
    Ogre::RenderWindow*		m_RenderWindow;
	Ogre::AnimationState    *m_AnimationState;
	Ogre::AnimationStateSet *m_AnimationStateSet;
	Ogre::Entity *m_HeadEntity;

	Ogre::AnimationState* m_ManualAnimState;
	Ogre::VertexPoseKeyFrame* m_ManualKeyFrame;
	bool m_First;

	void ExecuteCommand(ISpPhrase * Phrase);
	void EngineSetup(void);
	afx_msg void OnExpressionsMad();
	afx_msg void OnExpressionsSad();
	afx_msg void OnExpressionsHappy();
	afx_msg void OnExpressionsNeutral();
};

#ifndef _DEBUG  // debug version in DynamicGrammarView.cpp
inline CDynamicGrammarDoc* CDynamicGrammarView::GetDocument() const
   { return reinterpret_cast<CDynamicGrammarDoc*>(m_pDocument); }
#endif

