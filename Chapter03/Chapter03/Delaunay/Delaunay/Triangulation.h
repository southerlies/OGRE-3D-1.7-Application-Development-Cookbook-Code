#pragma once
#include "vertexcollection.h"
#include "EdgeCollection.h"

// CTriangulation command target

class CTriangulation : public CObject
{
public:

	enum CircleSide
	{
	LeftSide,
	RightSide
	};

	enum QueryType
	{
     BoundaryIn,
     BoundaryOut
	};

	enum PositionType
	{
	 None,
     Left,
     Right,
	 On
	};

	enum MemberType
	{
     Points,
	 Breaks,
	 Boundaries,
	 Holes,
	 Triangles,
	 AlphaHull,
	 SimpleHull
	};

	CTriangulation();
	virtual ~CTriangulation();
	CVertexCollection m_VertexCollection;
	CEdgeCollection m_EdgeCollection;
	int Draw();
	int VertexSerialize(CString Path,enum CArchive::Mode Mode);
	__declspec(dllexport)void Shuffle(void);
	__declspec(dllexport)void DelaunayIt(void);
	void FrameBuilder(void);
	void DelaunayStart(void);
	CEdge * VertexInsert(CVertex * Vertex, bool Force);
	CEdge * LocateTriangle(CVertex * Vertex, bool &OnEdge);
	void ClearPrinted(void);
	void Terminator(void);
	void TriangleSerialize(CString Path);
	void Eraser(void);
	void AllIncluded(void);

	CEdge *m_Anchor;
	bool m_HoleUnion;

	void VertexSwap(CObject ** Vertex1, CObject ** Vertex2);
	void VertexSwap(CObject * Vertex1, CObject * Vertex2);
	bool ImproveDelaunay(void);
	bool RightOn(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	bool NearEqual(double Number1, double Number2);
	bool LeftOn(CVertex * Vertex1 , CVertex * Vertex2 , CVertex * Vertex3);

	void FixVertex(CEdge* Edge, CVertex* Vertex);
	long m_TriangleNumber;
	long m_VertexNumber;
	bool m_IncrementalBuild;

	void FixInside(void);
public:

	double m_ScaleFactor;

	double FindElevation(CVertex *Vertex);

	void RestoreElevation(CObArray *Archive, CVertex *Vertex);
	void VisitClear(void);
    void VisitClear(long Visitor);
	CVertex& GetCircleCenter(CEdge * Edge, double Radius, enum CircleSide Side);

	double TriangleInterpolate(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex);
	bool OnLevel(CEdge * Edge, CVertex * Base, CVertex * Vertex);
};


