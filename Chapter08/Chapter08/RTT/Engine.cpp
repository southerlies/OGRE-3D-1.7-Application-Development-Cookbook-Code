#include "StdAfx.h"
#include "Engine.h"


CEngine::CEngine(void)
	: m_Root(NULL)
{
	TCHAR szPath[MAX_PATH];

	CString ApplicationName = "RTT.exe";

	GetModuleFileName(NULL, szPath, MAX_PATH);
    Ogre::ConfigFile OgreConfigFile;
	CString ApplicationPath(szPath);

	ApplicationPath = ApplicationPath.Left(ApplicationPath.GetLength() - ApplicationName.GetLength());

	m_Root = new Ogre::Root("", "", Ogre::String(ApplicationPath + "OgreInMfc.log")); 

	OgreConfigFile.load(Ogre::String(ApplicationPath + "ogre.cfg"), "\t:=", false);

	Ogre::String RenderSystemName;
	RenderSystemName = OgreConfigFile.getSetting("Render System");

		if (RenderSystemName == "Direct3D9 Rendering Subsystem")
		{
#ifdef _DEBUG
			m_Root->loadPlugin("RenderSystem_Direct3D9_d");
#else
		    m_Root->loadPlugin("RenderSystem_Direct3D9");
#endif
		}
		else			
		if(RenderSystemName == "OpenGL Rendering Subsystem")
		{
#ifdef _DEBUG
			m_Root->loadPlugin("RenderSystem_GL_d");
#else
		    m_Root->loadPlugin("RenderSystem_GL");
#endif
		}
		else
		{
#ifdef _DEBUG		 
			m_Root->loadPlugin("RenderSystem_Direct3D9_d");
#else
			m_Root->loadPlugin("RenderSystem_Direct3D9");
#endif
		}

#ifdef _DEBUG
		m_Root->loadPlugin("Plugin_ParticleFX_d");
#else
        m_Root->loadPlugin("Plugin_ParticleFX");
#endif

		Ogre::RenderSystemList RendersList = m_Root->getAvailableRenderers();
		m_Root->setRenderSystem(RendersList[0]);

    //
    // Initialize the system, but don't create a render window.
    //

		// Load resource paths from config file
        Ogre::ConfigFile cf;
		Ogre::String ResourcePath = ApplicationPath + "resources.cfg";
        cf.load(ResourcePath);

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;

		while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;

			for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
				archName = Ogre::String(ApplicationPath) + archName;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }

    m_Root->initialise(false);
}


CEngine::~CEngine(void)
{
}


Ogre::Root* CEngine::GetRoot(void)
{
	return m_Root;
}
