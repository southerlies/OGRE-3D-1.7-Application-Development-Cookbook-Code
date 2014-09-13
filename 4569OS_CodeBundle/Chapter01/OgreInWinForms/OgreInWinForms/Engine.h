#pragma once

#include "Ogre.h"

class CEngine
{
public:
	CEngine(HWND hWnd);
	~CEngine(void);

	Ogre::Root* m_Root;
	Ogre::SceneManager *m_SceneManager;
	Ogre::RenderWindow *m_RenderWindow;
	Ogre::Camera *m_Camera;
};

