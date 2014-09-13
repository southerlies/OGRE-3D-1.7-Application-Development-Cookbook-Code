// Cone.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicEffects.h"
#include "Cone.h"


// CCone

CCone::CCone()
{
	m_BaseSegments = 24;
	m_HeightSegments = 24;
	m_Radius = 10.0;
	m_Height = 20.0;
}

CCone::~CCone()
{
}


// CCone member functions


Ogre::ManualObject * CCone::CreateCone(Ogre::Real Intensity)
{
	Ogre::ManualObject *Cone = NULL;
	Ogre::SceneManager *SceneManager = Ogre::Root::getSingleton().getSceneManager("DynamicEffects");
	Cone = SceneManager->createManualObject("Cone");
	Cone->setDynamic(false);
    Cone->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / m_BaseSegments);
	Ogre::Real deltaHeight = m_Height/(Ogre::Real)m_HeightSegments;
	
	Ogre::Vector3 Normal = Ogre::Vector3(m_Radius, m_Height, 0.f).normalisedCopy();
	Ogre::Quaternion Quaternion;
	
	int Offset = 0;

	for (int HeightSegmentIndex = 0; HeightSegmentIndex <= m_HeightSegments; HeightSegmentIndex++)
	{
		Ogre::Real Radius = m_Radius * (1 - HeightSegmentIndex / (Ogre::Real)m_HeightSegments);
		
		for (int BaseSegmentIndex = 0; BaseSegmentIndex <= m_BaseSegments; BaseSegmentIndex++)
		{
			Ogre::Real x = Radius* cosf(BaseSegmentIndex * deltaAngle);
			Ogre::Real z = Radius * sinf(BaseSegmentIndex * deltaAngle);
			Cone->position(x, HeightSegmentIndex * deltaHeight, z);
			
			Cone->colour(Intensity, Intensity, 0.0, 1.0);
			Quaternion.FromAngleAxis(Ogre::Radian(-BaseSegmentIndex*deltaAngle), Ogre::Vector3::NEGATIVE_UNIT_Y);
			Cone->normal(Quaternion * Normal);
			Cone->textureCoord(BaseSegmentIndex / (Ogre::Real)m_BaseSegments, HeightSegmentIndex / (Ogre::Real)m_HeightSegments);

			if (HeightSegmentIndex != m_HeightSegments && BaseSegmentIndex != m_BaseSegments)
			{
				Cone->index(Offset + m_BaseSegments + 2);
				Cone->index(Offset);
				Cone->index(Offset + m_BaseSegments + 1);
				Cone->index(Offset + m_BaseSegments + 2);
				Cone->index(Offset + 1);
				Cone->index(Offset);
			}

			Offset++;
		}
	}

	int centerIndex = Offset;
	
	Cone->position(0,0,0);
	Cone->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
	Cone->textureCoord(0.0,1);
	Offset++;
	
	for (int BaseSegmentIndex=0; BaseSegmentIndex <= m_BaseSegments; BaseSegmentIndex++)
	{
		Ogre::Real x = m_Radius * cosf(BaseSegmentIndex*deltaAngle);
		Ogre::Real z = m_Radius * sinf(BaseSegmentIndex*deltaAngle);

		Cone->position(x, 0.0f, z);
		Cone->colour(Intensity, Intensity, 0.0, 0.0);
		Cone->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		Cone->textureCoord(BaseSegmentIndex/(Ogre::Real)m_BaseSegments,0.0);
		if (BaseSegmentIndex != m_BaseSegments)
		{
			Cone->index(centerIndex);
			Cone->index(Offset);
			Cone->index(Offset+1);
		}
		Offset++;
	}

	Cone->end();

	return Cone;
}


void CCone::UpdateCone(Ogre::Real Intensity)
{
}
