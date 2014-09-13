
// WeatherControl.h : main header file for the WeatherControl application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <sapi.h> 
#include "Engine.h"

// CWeatherControlApp:
// See WeatherControl.cpp for the implementation of this class
//

class CWeatherControlApp : public CWinAppEx
{
public:
	CWeatherControlApp();


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
	void InitEngine(void);
	CEngine* m_Engine;
    CComPtr<ISpVoice> m_cpVoice;

};

extern CWeatherControlApp theApp;
