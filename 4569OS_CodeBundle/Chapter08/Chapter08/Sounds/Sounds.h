
// Sounds.h : main header file for the Sounds application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"
#include <sapi.h>

// CSoundsApp:
// See Sounds.cpp for the implementation of this class
//

class CSoundsApp : public CWinAppEx
{
public:
	CSoundsApp();


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
	CEngine* m_Engine;
	CComPtr<ISpVoice> m_cpVoice;
	void InitEngine(void);
};

extern CSoundsApp theApp;
