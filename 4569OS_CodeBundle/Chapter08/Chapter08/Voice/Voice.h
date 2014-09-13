
// Voice.h : main header file for the Voice application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"
#include <sapi.h>


// CVoiceApp:
// See Voice.cpp for the implementation of this class
//

class CVoiceApp : public CWinAppEx
{
public:
	CVoiceApp();


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
	CEngine* m_Engine;
	CComPtr<ISpVoice> m_cpVoice;
	void InitEngine(void);
};

extern CVoiceApp theApp;
