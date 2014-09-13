
// PathDropping.h : main header file for the PathDropping application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"


// CPathDroppingApp:
// See PathDropping.cpp for the implementation of this class
//

class CPathDroppingApp : public CWinAppEx
{
public:
	CPathDroppingApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	void InitEngine(void);
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:

	CEngine *m_Engine;
};

extern CPathDroppingApp theApp;
