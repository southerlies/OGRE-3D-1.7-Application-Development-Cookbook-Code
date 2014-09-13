#pragma once

#include "OgreSerializer.h"

class StlFile;

class StlFileSerializer : public Ogre::Serializer
{
public:
	StlFileSerializer(void);
	~StlFileSerializer(void);

	void importStlFile(Ogre::DataStreamPtr &stream, StlFile *pDest);
};

