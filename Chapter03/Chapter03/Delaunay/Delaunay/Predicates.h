#pragma once
#include "Vertex.h"
#include "VertexCollection.h"

class CPredicates
{
public:
	CPredicates(void);
	~CPredicates(void);
	// Check for triangle direction
	static bool CounterClockWise(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	static bool CounterClockWise(CObArray &VertexCollection);
	// Calculation of signed area of triangle
	static double TriangleArea(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	static double PolygonArea(CObArray &VertexCollection);
	static double PolygonPerimeter(CObArray &VertexCollection);
	// Check if Vertex4 is inside circle created by three vertices
	static bool InCircle(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4);
	static double TriangleRatio(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	static bool ProperlyIntersect(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4);
    static bool Collinear(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4);
	static bool Left(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	static bool LeftOn(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	static bool Collinear(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3);
	static bool Collinear(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3, double epsilon);
	static bool Xor(bool x, bool y);
	
    static bool EqualSegments(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4);
	static bool Intersection(CVertex * VertexA, CVertex * VertexB, CVertex * VertexC, CVertex * VertexD, CVertex * Result);
	static double Distance2(CVertex * Vertex1, CVertex * Vertex2);
};
