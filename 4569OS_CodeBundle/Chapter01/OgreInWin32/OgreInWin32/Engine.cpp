#include "StdAfx.h"
#include "Engine.h"


CEngine::CEngine(HWND hWnd)
	: m_Root(NULL)
{
	char szPath[MAX_PATH];

	char ApplicationName[] = "OgreInWin32.exe";

	GetModuleFileName(NULL, szPath, MAX_PATH);
    Ogre::ConfigFile OgreConfigFile;
	char ApplicationPath[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	_splitpath(szPath, drive, dir, NULL, NULL); 

	strcpy(ApplicationPath, drive);
	strcat(ApplicationPath, dir);

	m_Root = new Ogre::Root("", "", Ogre::String(ApplicationPath + Ogre::String("OgreInWin32.log"))); 

	OgreConfigFile.load(Ogre::String(ApplicationPath + Ogre::String("ogre.cfg")), "\t:=", false);

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
		if (RenderSystemName == "OpenGL Rendering Subsystem")
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
		Ogre::String ResourcePath = ApplicationPath + Ogre::String("resources.cfg");
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
				archName = ApplicationPath + archName;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }

    m_Root->initialise(false);

	m_SceneManager = m_Root->createSceneManager(Ogre::ST_GENERIC, "Win32Ogre");

    //
    // Create a render window
    // This window should be the current window using the externalWindowHandle
    // value pair option.
    //

    Ogre::NameValuePairList parms;
    parms["externalWindowHandle"] = Ogre::StringConverter::toString((long)hWnd);
    parms["vsync"] = "true";

	RECT   rect;
    GetClientRect(hWnd, &rect);

	Ogre::RenderTarget *RenderWindow = NULL;

	try
	{
		m_RenderWindow = m_Root->createRenderWindow("Ogre in Win32", rect.right - rect.left, rect.bottom - rect.top, false, &parms);
	}
    catch(...)
	{
		MessageBox(hWnd, "Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = m_SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
	m_Camera->setAspectRatio(Ogre::Real(rect.right - rect.left) / Ogre::Real(rect.bottom - rect.top));
		
	Ogre::Entity *RobotEntity = m_SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox RobotBox = RobotEntity->getBoundingBox();
	Ogre::Vector3 RobotCenter = RobotBox.getCenter();
	m_Camera->lookAt(RobotCenter);

	m_Root->renderOneFrame();
}


CEngine::~CEngine(void)
{
}


//Ogre::Root* CEngine::GetRoot(void)
//{
//	return m_Root;
//}
