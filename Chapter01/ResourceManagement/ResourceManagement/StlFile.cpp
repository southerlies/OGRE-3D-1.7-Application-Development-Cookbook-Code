#pragma once

#include "StdAfx.h"
#include "StlFile.h"
#include "Ogre.h"
#include "OgreResourceManager.h"
#include "StlFileSerializer.h"

StlFile::StlFile(Ogre::ResourceManager *creator, 
			const Ogre::String &name, 
			Ogre::ResourceHandle handle, 
			const Ogre::String &group, 
			bool isManual, 
			Ogre::ManualResourceLoader *loader
		   ) : Ogre::Resource(creator, name, handle, group, isManual,loader)
{
	bool result = createParamDictionary("StlFile");
}

StlFile::~StlFile(void)
{
	unload();
}

 void StlFile::loadImpl()
 {
	 if(Ogre::MeshManager::getSingleton().resourceExists(this->mName)) {
		 setMesh(Ogre::MeshManager::getSingleton().getByName(this->mName));
	 }
	 else {
		 StlFileSerializer serializer;
		 Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
		 serializer.importStlFile(stream, this);
	 }
 }
 
 void StlFile::unloadImpl()
 {
     /* If you were storing a pointer to an object, then you would check the pointer here,
     and if it is not NULL, you would destruct the object and set its pointer to NULL again.
     */

	 // our mesh pointer will be released automatically
 }
 
 size_t StlFile::calculateSize() const
 {
	 // our mesh size is already calculated by the mesh manager
	 return 0;
 }
 
 void StlFile::setMesh(Ogre::MeshPtr mesh)
 {
	 mMesh = mesh;
 }
 Ogre::MeshPtr StlFile::getMesh() const
 {
	 return mMesh;
 }