#pragma once

#include "Ogre.h"

class CEngine
{
public:
	CEngine(void);
	~CEngine(void);

	Ogre::Root* m_Root;
	Ogre::Root* GetRoot(void);
};

