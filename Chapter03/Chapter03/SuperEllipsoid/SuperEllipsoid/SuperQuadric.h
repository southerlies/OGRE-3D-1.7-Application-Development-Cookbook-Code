#pragma once

// CSuperQuadric command target

class CSuperQuadric : public CObject
{
public:
	CSuperQuadric();
	virtual ~CSuperQuadric();

	double m_VertFactor;
	double m_HorFactor;
	Ogre::ManualObject* m_ManualObject;

	double SuperCos(double Angle, double Power);
	double SuperSin(double Angle, double Power);
	double CalculateX(double Theta, double Phi);
	double CalculateY(double Theta, double Phi);
	double CalculateZ(double Phi);
	void DrawMesh(void);
	void UpdateMesh(void);
};


