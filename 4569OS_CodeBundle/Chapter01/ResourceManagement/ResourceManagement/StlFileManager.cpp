#include "StdAfx.h"
#include "Ogre.h"
#include "StlFileManager.h"
#include "Engine.h"
#include "ResourceManagement.h"
#include "OgreResourceGroupManager.h"

StlFileManager::~StlFileManager(void)
{
     Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

 template<> StlFileManager *Ogre::Singleton<StlFileManager>::ms_Singleton = 0;
 
 StlFileManager *StlFileManager::getSingletonPtr()
 {
     return ms_Singleton;
 }
 
 StlFileManager &StlFileManager::getSingleton()
 {  
     assert(ms_Singleton);  
     return(*ms_Singleton);
 }
 
 StlFileManager::StlFileManager()
 {
     mResourceType = "StlFile";
 
     mLoadOrder = 30.0f;
 
     // this is how we register the ResourceManager with OGRE
     Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
 }
 
 StlFilePtr StlFileManager::load(const Ogre::String &name, const Ogre::String &group)
 {
     StlFilePtr stlFile = getByName(name, group);
 
     if (stlFile.isNull())
         stlFile = create(name, group);
 
     stlFile->load();
     return stlFile;
 }
 
 Ogre::Resource *StlFileManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle, 
                                             const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader, 
                                             const Ogre::NameValuePairList *createParams)
{
	StlFile* File = OGRE_NEW StlFile(this, name, handle, group, isManual, loader);
    return File;
}
	
Ogre::Entity* StlFileManager::createEntity(const Ogre::String &entityName, const Ogre::String &meshName)
{
	// load the resource first
	StlFilePtr stlFile = load(meshName, "StlFile");
	if(stlFile.isNull()) {
		return NULL;
	}

	// get the first available scene manager
	Ogre::SceneManager* sceneManager = Ogre::Root::getSingleton().getSceneManagerIterator().begin()->second;
	return sceneManager->createEntity(meshName);
}