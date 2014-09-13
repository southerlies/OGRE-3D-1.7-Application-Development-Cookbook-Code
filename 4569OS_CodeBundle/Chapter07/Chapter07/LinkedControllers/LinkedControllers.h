
// LinkedControllers.h : main header file for the LinkedControllers application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"


// CLinkedControllersApp:
// See LinkedControllers.cpp for the implementation of this class
//

class CLinkedControllersApp : public CWinAppEx
{
public:
	CLinkedControllersApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:

	void InitEngine(void);
	CEngine *m_Engine;
};

extern CLinkedControllersApp theApp;
