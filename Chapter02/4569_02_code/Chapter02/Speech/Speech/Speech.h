
// Speech.h : main header file for the Speech application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <sapi.h> 
#include "Engine.h"

// CSpeechApp:
// See Speech.cpp for the implementation of this class
//

class CSpeechApp : public CWinAppEx
{
public:
	CSpeechApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:

	CEngine* m_Engine;
    CComPtr<ISpVoice> m_cpVoice;

	void InitEngine(void);
};

extern CSpeechApp theApp;
