#pragma once

#include "Ogre.h"
#include "StlFileManager.h"

class CEngine
{
public:
	CEngine(void);
	~CEngine(void);

	Ogre::Root* m_Root;
	Ogre::Root* GetRoot(void);
	StlFileManager* m_StlFileManager;
};

