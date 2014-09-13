
// WeatherControlView.h : interface of the CWeatherControlView class
//

#pragma once
#include "WeatherControlDoc.h"

//#include "WeatherControlDlg.h"
#include "RainControlDlg.h"
#include "SnowControlDlg.h"
#include "SkyControlDlg.h"
#include "SunControlDlg.h"
#include "FogControlDlg.h"

class CWeatherControlView : public CView
{
protected: // create from serialization only
	CWeatherControlView();
	DECLARE_DYNCREATE(CWeatherControlView)

// Attributes
public:
	CWeatherControlDoc* GetDocument() const;

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
	virtual ~CWeatherControlView();
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

	//CWeatherControlDlg *m_WeatherControlDlg;
	CRainControlDlg *m_RainControlDlg;
	CSnowControlDlg *m_SnowControlDlg;
	CSkyControlDlg *m_SkyControlDlg;
	CFogControlDlg *m_FogControlDlg;
	CSunControlDlg *m_SunControlDlg;

	Ogre::ParticleSystem *m_Rain;
	Ogre::ParticleSystem *m_Sun;
	Ogre::ParticleSystem *m_Snow;

//	afx_msg void OnWeatherControlWeatherControl();
	afx_msg void OnPaint();
	void EngineSetup(void);
	afx_msg void OnWeatherControlRain();
	afx_msg void OnWeatherControlSnow();
	afx_msg void OnWeatherControlFog();
	afx_msg void OnWeatherControlSky();
	afx_msg void OnWeatherControlSun();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnBnClickedAction();
//	afx_msg void OnAction();
//	afx_msg void OnBnClickedAction();
//	afx_msg void OnAction();
};

#ifndef _DEBUG  // debug version in WeatherControlView.cpp
inline CWeatherControlDoc* CWeatherControlView::GetDocument() const
   { return reinterpret_cast<CWeatherControlDoc*>(m_pDocument); }
#endif

