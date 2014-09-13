
// MirrorView.h : interface of the CMirrorView class
//

#pragma once


class CMirrorView : public CView
{
protected: // create from serialization only
	CMirrorView();
	DECLARE_DYNCREATE(CMirrorView)

// Attributes
public:
	CMirrorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMirrorView();
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
	Ogre::Camera* m_MirrorCamera;
    Ogre::MovablePlane* m_MirrorPlane;
    Ogre::Entity* m_MirrorEntity;
	Ogre::SceneManager *m_SceneManager;
	Ogre::AnimationState *m_WalkAnimation;

	void EngineSetup(void);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

void createMirror(Ogre::String planeName,
                        Ogre::String texName,
                        Ogre::Camera* MirrorCamera,
                        Ogre::Real sizeX,
                        Ogre::Real sizeY,
                        Ogre::Vector3 position
						);
afx_msg void OnAnimationStart();
afx_msg void OnAnimationStop();
};

#ifndef _DEBUG  // debug version in MirrorView.cpp
inline CMirrorDoc* CMirrorView::GetDocument() const
   { return reinterpret_cast<CMirrorDoc*>(m_pDocument); }
#endif

