
// ResourceManagement.h : main header file for the ResourceManagement application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"
#include "StlFileManager.h"

// CResourceManagementApp:
// See ResourceManagement.cpp for the implementation of this class
//

class CResourceManagementApp : public CWinAppEx
{
public:
	CResourceManagementApp();


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

	CEngine* m_Engine;
	void InitEngine(void);
};

extern CResourceManagementApp theApp;
