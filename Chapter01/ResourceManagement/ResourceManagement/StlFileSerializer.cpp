#include "StdAfx.h"
#include "Ogre.h"
#include "StlFileSerializer.h"
#include "StlFile.h"


StlFileSerializer::StlFileSerializer(void)
{
}


StlFileSerializer::~StlFileSerializer(void)
{
}

 void StlFileSerializer::importStlFile(Ogre::DataStreamPtr &stream, StlFile *pDest)
 {
	Ogre::SceneManager *sceneManager = Ogre::Root::getSingleton().getSceneManagerIterator().begin()->second;

	Ogre::ManualObject* ManualObject = sceneManager->createManualObject(pDest->getName());
	ManualObject->setDynamic(false);
    ManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float x,y,z,nx,ny,nz;

	// first line is solid name (skip)
	stream->getLine();

	int TriangleIndex = 0;
	while(!stream->eof()) {
		// facet normal nx ny nz
		int ret = sscanf(stream->getLine().c_str(), "%*s %*s %f %f %f\n", &nx, &ny, &nz); 
		if (ret!=3) continue;

		// skip outer loop declaration
		stream->getLine();
		
		for(int i = 0; i < 3; ++i) {
			//vertex x y z
			ret = sscanf(stream->getLine().c_str(), "%*s %f %f %f\n", &x,  &y,  &z); 
			if (ret != 3) return;

			ManualObject->position(x, y, z);
			ManualObject->normal(nx, ny, nz);
			ManualObject->colour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
		}

		ManualObject->triangle(TriangleIndex * 3 + 0, TriangleIndex * 3 + 1, TriangleIndex * 3 + 2);
		TriangleIndex++;

		// skip outer loop end
		stream->getLine();

		// skip facet end
		stream->getLine();
	}

	ManualObject->end();

	pDest->setMesh( ManualObject->convertToMesh(pDest->getName()) );
 }
