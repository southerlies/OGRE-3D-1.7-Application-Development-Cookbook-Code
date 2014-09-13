#pragma once
#include "Vertex.h"

class CTriangulation;
// CVertexCollection command target

class CVertexCollection : public CObject
{
public:
	CVertexCollection();
	virtual ~CVertexCollection();
	CObArray m_Collection;
	void Serialize(CArchive& Archive, CTriangulation *Triangulation);
	void Shuffle(void);
	long m_VertexNumber;

	double m_DeltaX;
    double m_DeltaY;

	CVertex m_Min;
	CVertex m_Max;
	};


