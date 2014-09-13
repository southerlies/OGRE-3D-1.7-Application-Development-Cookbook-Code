
// MouseInput.h : main header file for the MouseInput application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"

// CMouseInputApp:
// See MouseInput.cpp for the implementation of this class
//

class CMouseInputApp : public CWinAppEx
{
public:
	CMouseInputApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
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

extern CMouseInputApp theApp;
