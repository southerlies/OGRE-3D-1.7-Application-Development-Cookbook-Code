
// DynamicGrammar.h : main header file for the DynamicGrammar application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <sphelper.h>
#include "Engine.h"
// CDynamicGrammarApp:
// See DynamicGrammar.cpp for the implementation of this class
//

class CDynamicGrammarApp : public CWinAppEx
{
public:
	CDynamicGrammarApp();


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

	CComPtr<ISpRecognizer> m_cpEngine;
	CComPtr<ISpRecoContext> m_cpRecoCtxt;
	CComPtr<ISpRecoGrammar> m_cpCmdGrammar;
	bool InitializeSpeechRecognition(void);
	void InitEngine(void);
	CEngine *m_Engine;
};

extern CDynamicGrammarApp theApp;
