
// DynamicTexture.h : main header file for the DynamicTexture application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"


// CDynamicTextureApp:
// See DynamicTexture.cpp for the implementation of this class
//

class CDynamicTextureApp : public CWinAppEx
{
public:
	CDynamicTextureApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

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
	void InitEngine(void);
};

extern CDynamicTextureApp theApp;
