#pragma once

// CVertex command target

class CVertex : public CObject
{
public:

	enum VertexType
	{
     BoundingBox = 0,
	 Standalone,
	 Break,
	 Hole,
	 Artificial,
	 Boundary,
	 Shoulder,
	 Jump
	};

	// Constructor with initialization
	CVertex();
	virtual ~CVertex();
	// Vertex coordinates
	double m_Coordinate[3];
	CVertex(double x, double y, double z);
	CVertex(double x, double y, double z, VertexType);
	//CVertex operator+(CVertex &Vertex); 
	CVertex& operator=(const CVertex& Vertex);

	VertexType m_VertexType;
	INT_PTR m_Index;
	bool m_Cut;
	bool m_ToDelete;
};


