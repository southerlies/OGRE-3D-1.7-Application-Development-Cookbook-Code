
// Walking.h : main header file for the Walking application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"


// CWalkingApp:
// See Walking.cpp for the implementation of this class
//

class CWalkingApp : public CWinAppEx
{
public:
	CWalkingApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
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

extern CWalkingApp theApp;
