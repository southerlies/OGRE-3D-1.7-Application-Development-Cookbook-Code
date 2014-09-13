
// TerrainWalking.h : main header file for the TerrainWalking application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"


// CTerrainWalkingApp:
// See TerrainWalking.cpp for the implementation of this class
//

class CTerrainWalkingApp : public CWinAppEx
{
public:
	CTerrainWalkingApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
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

extern CTerrainWalkingApp theApp;
