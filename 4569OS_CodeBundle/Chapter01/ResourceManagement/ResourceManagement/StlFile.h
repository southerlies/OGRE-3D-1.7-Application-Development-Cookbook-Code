#pragma once 

#include "Ogre.h"
#include "OgreResourceManager.h"

using namespace Ogre;

class StlFile :	public Ogre::Resource
{
protected:

	void loadImpl();
	void unloadImpl();
	size_t calculateSize() const;
	
	Ogre::MeshPtr mMesh;

public:

	StlFile(Ogre::ResourceManager *creator, 
			const Ogre::String &name, 
			Ogre::ResourceHandle handle, 
			const Ogre::String &group, 
			bool isManual = false, 
			Ogre::ManualResourceLoader *loader = 0
		   );

	virtual ~StlFile();

	void setMesh(Ogre::MeshPtr mesh);
    Ogre::MeshPtr getMesh() const;
};

class StlFilePtr : public Ogre::SharedPtr<StlFile>
{
 public:

	 StlFilePtr() : Ogre::SharedPtr<StlFile>() {}
     explicit StlFilePtr(StlFile *rep) : Ogre::SharedPtr<StlFile>(rep) {}
     StlFilePtr(const StlFilePtr &r) : Ogre::SharedPtr<StlFile>(r) {} 
     StlFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<StlFile>()
     {
         if( r.isNull() )
             return;
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<StlFile*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
     }
 
     /// Operator used to convert a ResourcePtr to a StlFilePtr
     StlFilePtr& operator=(const Ogre::ResourcePtr& r)
     {
         if(pRep == static_cast<StlFile*>(r.getPointer()))
             return *this;
         release();
         if( r.isNull() )
             return *this; // resource ptr is null, so the call to release above has done all we need to do.
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<StlFile*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
         return *this;
     }
};
