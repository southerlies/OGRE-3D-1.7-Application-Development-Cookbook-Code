#pragma once

// CCone command target

class CCone : public CObject
{
public:
	CCone();
	virtual ~CCone();

	int m_BaseSegments;
	int m_HeightSegments;
	Ogre::Real m_Radius;
	Ogre::Real m_Height;

	Ogre::ManualObject * CreateCone(Ogre::Real Intensity, const Ogre::String& name = "", const Ogre::String& materialName = "", Ogre::Real fadeAmount = 0.0);
	void UpdateCone(Ogre::Real Intensity);
};


