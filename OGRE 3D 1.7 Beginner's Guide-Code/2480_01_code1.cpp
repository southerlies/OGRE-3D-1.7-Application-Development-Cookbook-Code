#include "Ogre\ExampleApplication.h"


class Example1 : public ExampleApplication
{
public:

	void createScene()
	{
		Ogre::Entity* ent = mSceneMgr->createEntity("MyEntity","Sinbad.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(ent);
	}
};

int main (void)
{
	Example1 app;
	app.go();
	return 0;
}