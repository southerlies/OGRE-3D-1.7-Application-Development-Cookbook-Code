#include "Robot3.h"
#include "Ogre.h"
#include "OgrePlugin.h"

Robot3Plugin *Robot;

extern "C" __declspec(dllexport) void dllStartPlugin()
{
	Robot = OGRE_NEW Robot3Plugin();
	Ogre::Root::getSingleton().installPlugin(Robot);
}

extern "C" __declspec(dllexport) void dllStopPlugin()
{
	Ogre::Root::getSingleton().uninstallPlugin(Robot); 
	OGRE_DELETE Robot;
}