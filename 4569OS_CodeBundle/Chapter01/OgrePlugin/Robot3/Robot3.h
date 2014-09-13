#include "Ogre.h"
#include "OgrePlugin.h"

	class Robot3Plugin : public Ogre::Plugin
	{
	public:
		Robot3Plugin()
		{
			m_Name = "Robot";
		}

		const Ogre::String& getName() const 
		{
			return m_Name;
		}

		void install()
		{

		}

		void initialise()
		{
			Ogre::SceneManager *SceneManager = Ogre::Root::getSingleton().getSceneManager("OgrePlugin");
			Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
			Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode();
			RobotNode->attachObject(RobotEntity);

			Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
			Ogre::Vector3 Center = Box.getCenter();
			Ogre::Camera *Camera = SceneManager->getCamera("Camera");
			Camera->lookAt(Center);

			Ogre::Root::getSingleton().renderOneFrame();
		}

		void shutdown()
		{
		}

		void uninstall()
		{
		}

		Ogre::String m_Name;
	};
