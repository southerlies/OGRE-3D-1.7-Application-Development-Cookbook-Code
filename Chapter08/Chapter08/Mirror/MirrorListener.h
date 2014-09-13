#pragma once
#include "c:\users\ilya\documents\visual studio 2010\projects\recipes\ogresdk_vc10_v1-7-1\include\ogre\ogrerendertargetlistener.h"
class MirrorListener : public Ogre::RenderTargetListener
{
public:

	MirrorListener():Ogre::RenderTargetListener()
	{
		m_PlaneEntity = NULL;
	}

    void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
		if (m_PlaneEntity != NULL)
		  m_PlaneEntity->setVisible(false);

    }
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
		if (m_PlaneEntity != NULL)
          m_PlaneEntity->setVisible(true);
    }

	Ogre::Entity *m_PlaneEntity;

};

