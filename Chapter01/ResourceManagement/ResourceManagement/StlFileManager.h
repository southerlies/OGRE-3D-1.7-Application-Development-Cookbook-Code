#pragma once

#include "OgreResourceManager.h"
#include "StlFile.h"

class StlFileManager : public Ogre::ResourceManager, public Ogre::Singleton<StlFileManager>
{
public:
	StlFileManager(void);
	~StlFileManager(void);

     virtual StlFilePtr load(const Ogre::String &name, const Ogre::String &group);
	 Ogre::Entity* createEntity(const Ogre::String &entityName, const Ogre::String &meshName);

     static StlFileManager &getSingleton();
     static StlFileManager *getSingletonPtr();

protected:
 
     Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle, 
         const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader, 
         const Ogre::NameValuePairList *createParams);

};

