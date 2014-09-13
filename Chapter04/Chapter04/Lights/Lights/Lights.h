
// Lights.h : main header file for the Lights application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"

// CLightsApp:
// See Lights.cpp for the implementation of this class
//

class CLightsApp : public CWinAppEx
{
public:
	CLightsApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	void InitEngine(void);
	CEngine* m_Engine;
};

extern CLightsApp theApp;
