
// SceneEditor.h : main header file for the SceneEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Engine.h"

// CSceneEditorApp:
// See SceneEditor.cpp for the implementation of this class
//

class CSceneEditorApp : public CWinApp
{
public:
	CSceneEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:

	void InitEngine(void);
	CEngine *m_Engine;
};

extern CSceneEditorApp theApp;
