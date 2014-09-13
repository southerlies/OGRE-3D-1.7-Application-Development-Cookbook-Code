// SuperQuadric.cpp : implementation file
//

#include "stdafx.h"
#include "SuperEllipsoid.h"
#include "SuperQuadric.h"

#include <math.h>
#include <float.h>

// CSuperQuadric

CSuperQuadric::CSuperQuadric()
{
	m_VertFactor = 1.0;
	m_HorFactor = 1.0;
}

CSuperQuadric::~CSuperQuadric()
{
}


// CSuperQuadric member functions


double CSuperQuadric::SuperCos(double Angle, double Power)
{
	return _copysign(pow(fabs(cos(Angle)), Power), cos(Angle));
}

double CSuperQuadric::SuperSin(double Angle, double Power)
{
	return _copysign(pow(fabs(sin(Angle)), Power), sin(Angle));
} 

double CSuperQuadric::CalculateX(double Theta, double Phi)
{
	return SuperCos(Phi, m_HorFactor) * SuperCos(Theta, m_VertFactor);
}

double CSuperQuadric::CalculateY(double Theta, double Phi)
{
	return SuperCos(Phi, m_HorFactor) * SuperSin(Theta, m_VertFactor);
}

double CSuperQuadric::CalculateZ(double Phi)
{
	return SuperSin(Phi, m_HorFactor);
}


void CSuperQuadric::DrawMesh(void)
{
	double Pi = acos(-1.0);
	double TwoPi = 2.0 * Pi;
	double Theta;
	double Phi;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	double Delta = TwoPi / 72;

	int VertexIndex = 0;
	int TriangleIndex = 0;

	Ogre::Camera *Camera = NULL;
	CEngine* Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager* SceneManager = Root->getSceneManager("SuperEllipsoid");
 
	m_ManualObject = SceneManager->createManualObject("Superellipsoid");
	m_ManualObject->setDynamic(true);
    m_ManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

 for (Theta = -Pi; Theta <= Pi; Theta += Delta)
	for (Phi = -0.5 * Pi; Phi <= 0.5 * Pi; Phi += Delta)
	{
     x = CalculateX(Theta, Phi);
	 y = CalculateY(Theta, Phi);
	 z = CalculateZ(Phi);
  
	 m_ManualObject->position(x, y, z);

     x = CalculateX(Theta + Delta, Phi);
	 y = CalculateY(Theta + Delta, Phi);
	 z = CalculateZ(Phi);
  
	 m_ManualObject->position(x, y, z);
		 
	 x = CalculateX(Theta + Delta, Phi + Delta);
	 y = CalculateY(Theta + Delta, Phi + Delta);
	 z = CalculateZ(Phi + Delta);
  
	 m_ManualObject->position(x, y, z);

     m_ManualObject->triangle(TriangleIndex * 3 + 0, TriangleIndex * 3 + 1, TriangleIndex * 3 + 2);
	 TriangleIndex++;

     x = CalculateX(Theta, Phi);
	 y = CalculateY(Theta, Phi);
	 z = CalculateZ(Phi);
  
	 m_ManualObject->position(x, y, z);

	 x = CalculateX(Theta + Delta, Phi + Delta);
	 y = CalculateY(Theta + Delta, Phi + Delta);
	 z = CalculateZ(Phi + Delta);
  
	 m_ManualObject->position(x, y, z);
     
	 x = CalculateX(Theta, Phi + Delta);
	 y = CalculateY(Theta, Phi + Delta);
	 z = CalculateZ(Phi + Delta);
  
	 m_ManualObject->position(x, y, z);
	
     m_ManualObject->triangle(TriangleIndex * 3 + 0, TriangleIndex * 3 + 1, TriangleIndex * 3 + 2);
	 TriangleIndex++;
	} 

	m_ManualObject->end();

	Ogre::SceneNode *SuperEllipsoidNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	SuperEllipsoidNode->attachObject(m_ManualObject);
	Ogre::AxisAlignedBox Box = m_ManualObject->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	Camera = SceneManager->getCamera("Camera");
	Camera->lookAt(Center);
	Root->renderOneFrame();
}


void CSuperQuadric::UpdateMesh(void)
{
	double Pi = acos(-1.0);
	double TwoPi = 2.0 * Pi;
	double Theta;
	double Phi;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	double Delta = TwoPi / 72;

	CEngine* Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
 
	m_ManualObject->beginUpdate(0);

 for (Theta = -Pi; Theta <= Pi; Theta += Delta)
	for (Phi = -0.5 * Pi; Phi <= 0.5 * Pi; Phi += Delta)
	{
     x = CalculateX(Theta, Phi);
	 y = CalculateY(Theta, Phi);
	 z = CalculateZ(Phi);
  
	 m_ManualObject->position(x, y, z);

     x = CalculateX(Theta + Delta, Phi);
	 y = CalculateY(Theta + Delta, Phi);
	 z = CalculateZ(Phi);
  
	 m_ManualObject->position(x, y, z);
		 
	 x = CalculateX(Theta + Delta, Phi + Delta);
	 y = CalculateY(Theta + Delta, Phi + Delta);
	 z = CalculateZ(Phi + Delta);
  
	 m_ManualObject->position(x, y, z);

     x = CalculateX(Theta, Phi);
	 y = CalculateY(Theta, Phi);
	 z = CalculateZ(Phi);
  
	 m_ManualObject->position(x, y, z);

	 x = CalculateX(Theta + Delta, Phi + Delta);
	 y = CalculateY(Theta + Delta, Phi + Delta);
	 z = CalculateZ(Phi + Delta);
  
	 m_ManualObject->position(x, y, z);
     
	 x = CalculateX(Theta, Phi + Delta);
	 y = CalculateY(Theta, Phi + Delta);
	 z = CalculateZ(Phi + Delta);
  
	 m_ManualObject->position(x, y, z);
	} 

	m_ManualObject->end();
	Root->renderOneFrame();
}
