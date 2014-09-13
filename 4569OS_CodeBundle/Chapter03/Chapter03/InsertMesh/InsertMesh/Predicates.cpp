#include "StdAfx.h"
#include "Predicates.h"
#include <cmath>
#include <stdlib.h>

CPredicates::CPredicates(void)
{
}

CPredicates::~CPredicates(void)
{
}

// Check for triangle direction
bool CPredicates::CounterClockWise(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 return TriangleArea(Vertex1, Vertex2, Vertex3) > 0.0;
}

// Calculation of signed area of triangle
double CPredicates::TriangleArea(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 double a0 = Vertex1->m_Coordinate[0];
 double a1 = Vertex1->m_Coordinate[1];
 double b0 = Vertex2->m_Coordinate[0];
 double b1 = Vertex2->m_Coordinate[1];
 double c0 = Vertex3->m_Coordinate[0];
 double c1 = Vertex3->m_Coordinate[1];
 
 double area = a0 * b1 - a1 * b0 + a1 * c0 - a0 * c1 + b0 * c1 - c0 * b1;

 return area;
}

// Check if Vertex4 is inside circle created by three vertices
bool CPredicates::InCircle(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4)
{
 double x1 = Vertex1->m_Coordinate[0] - Vertex4->m_Coordinate[0];
 double x2 = Vertex2->m_Coordinate[0] - Vertex4->m_Coordinate[0];
 double x3 = Vertex3->m_Coordinate[0] - Vertex4->m_Coordinate[0];

 double y1 = Vertex1->m_Coordinate[1] - Vertex4->m_Coordinate[1];
 double y2 = Vertex2->m_Coordinate[1] - Vertex4->m_Coordinate[1];
 double y3 = Vertex3->m_Coordinate[1] - Vertex4->m_Coordinate[1];

 double d1 = x1 * x1 + y1 * y1;
 double d2 = x2 * x2 + y2 * y2;
 double d3 = x3 * x3 + y3 * y3;

 double Det = d1 * (x2 * y3 - x3 * y2) + 
	          d2 * (x3 * y1 - x1 * y3) +
			  d3 * (x1 * y2 - x2 * y1);

 return Det > 0.0;
}

//Returns triangle's circumradius-to-shortest edge ratio. 
double CPredicates::TriangleRatio(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 double D = 0.0;
 double r = 0.0;
 
 double l1 = 0.0;
 double l2 = 0.0;
 double l3 = 0.0;
 double lmin = 0.0;

 double p0 = 0.0;
 double p1 = 0.0;

 double a0 = 0.0;
 double a1 = 0.0;

 double b0 = Vertex1->m_Coordinate[0] - Vertex2->m_Coordinate[0];
 double b1 = Vertex1->m_Coordinate[1] - Vertex2->m_Coordinate[1];

 double c0 = Vertex1->m_Coordinate[0] - Vertex3->m_Coordinate[0];
 double c1 = Vertex1->m_Coordinate[1] - Vertex3->m_Coordinate[1];

 D = 2 * (a1 * c0 + b1 * a0 - b1 * c0 - a1 * b0 - c1 * a0 + c1 * b0);

 p0 = (b1 * a0 * a0 - c1 * a0 * a0 - b1 * b1 * a1 + c1 * c1 * a1 + b0 * b0 * c1 + a1 * a1 * b1 +
	   c0 * c0 * a1 - c1 * c1 * b1 - c0 * c0 * b1 - b0 * b0 * a1 + b1 * b1 * c1 - a1 * a1 * c1
	   )/D;

 p1 = (a0 * a0 * c0+ a1 * a1 * c0 + b0 * b0 * a0 - b0 * b0 * c0 + b1 * b1 * a0 - b1 * b1 * c0 -
	   a0 * a0 * b0 - a1 * a1 * b0 - c0 * c0 * a0 + c0 * c0 * b0 - c1 * c1 * a0 +c1 * c1 * b0
	   )/D;

 r = sqrt((a0 - p0) * (a0 - p0) + (a1 - p1) * (a1 - p1));

 l1 = sqrt((a0 - b0) * (a0 - b0) + (a1 - b1) * (a1 - b1));
 l2 = sqrt((c0 - b0) * (c0 - b0) + (c1 - b1) * (c1 - b1));
 l3 = sqrt((a0 - c0) * (a0 - c0) + (a1 - c1) * (a1 - c1));

 lmin = __min(__min(l1, l2), l3);
 return r/lmin;
}

bool CPredicates::ProperlyIntersect(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4)
{
 if (
	 Collinear(Vertex1, Vertex2, Vertex3) ||
	 Collinear(Vertex1, Vertex2, Vertex4) ||
	 Collinear(Vertex3, Vertex4, Vertex1) ||
	 Collinear(Vertex3, Vertex4, Vertex2) 
	 )
 return false;

 return 
	 Xor(Left(Vertex1, Vertex2, Vertex3), Left(Vertex1, Vertex2, Vertex4)) &&
	 Xor(Left(Vertex3, Vertex4, Vertex1), Left(Vertex3, Vertex4, Vertex2));
}

bool CPredicates::Collinear(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4)
{
 if (
	 Collinear(Vertex1, Vertex2, Vertex3) &&
	 Collinear(Vertex1, Vertex2, Vertex4) &&
	 Collinear(Vertex3, Vertex4, Vertex1) &&
	 Collinear(Vertex3, Vertex4, Vertex2) 
	 )
 return true;

 return false;
}

bool CPredicates::Left(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 return TriangleArea(Vertex1, Vertex2, Vertex3) > 0;
}

bool CPredicates::LeftOn(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 return TriangleArea(Vertex1, Vertex2, Vertex3) >= 0;
}

bool CPredicates::Collinear(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 return fabs(TriangleArea(Vertex1, Vertex2, Vertex3)) < 0.00001;
}

bool CPredicates::Collinear(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3, double epsilon)
{
 return fabs(TriangleArea(Vertex1, Vertex2, Vertex3)) < epsilon;
}

bool CPredicates::Xor(bool x, bool y)
{
 return !x ^ !y;
}

bool CPredicates::CounterClockWise(CObArray &VertexCollection)
{
 return PolygonArea(VertexCollection) > 0.0;
}

double CPredicates::PolygonArea(CObArray &VertexCollection)
{
 double Area = 0;
 CVertex *Vertex = (CVertex *)VertexCollection[0];

 for (int VertexIndex = 1; VertexIndex < VertexCollection.GetCount() - 1 ; VertexIndex++)
 {
  CVertex *Vertex1 = (CVertex *)VertexCollection[VertexIndex];
  CVertex *Vertex2 = (CVertex *)VertexCollection[VertexIndex + 1];

  Area += TriangleArea(Vertex, Vertex1, Vertex2);
 }

 return Area;
}

double CPredicates::PolygonPerimeter(CObArray &VertexCollection)
{
 double Perimeter = 0;

 for (int VertexIndex = 0; VertexIndex < VertexCollection.GetCount() - 1; VertexIndex++)
 {
  CVertex *Vertex1 = (CVertex *)VertexCollection[VertexIndex];
  CVertex *Vertex2 = (CVertex *)VertexCollection[VertexIndex + 1];

  Perimeter += sqrt(Distance2(Vertex1, Vertex2));
 }

 return Perimeter;
}

bool CPredicates::Intersection(CVertex * VertexC, CVertex * VertexD, CVertex * VertexA, CVertex * VertexB, CVertex * Result)
{
 double s;
 double t;
 double denom;

 denom = 
	 VertexA->m_Coordinate[0] * (VertexD->m_Coordinate[1] - VertexC->m_Coordinate[1]) +
	 VertexB->m_Coordinate[0] * (VertexC->m_Coordinate[1] - VertexD->m_Coordinate[1]) +
	 VertexD->m_Coordinate[0] * (VertexB->m_Coordinate[1] - VertexA->m_Coordinate[1]) +
	 VertexC->m_Coordinate[0] * (VertexA->m_Coordinate[1] - VertexB->m_Coordinate[1]) ;

 if (denom == 0.0)
	return false;

 s = ( 
	 VertexA->m_Coordinate[0] * (VertexD->m_Coordinate[1] - VertexC->m_Coordinate[1]) +
	 VertexC->m_Coordinate[0] * (VertexA->m_Coordinate[1] - VertexD->m_Coordinate[1]) +
	 VertexD->m_Coordinate[0] * (VertexC->m_Coordinate[1] - VertexA->m_Coordinate[1])
 ) / denom;

 t = -(
	 VertexA->m_Coordinate[0] * (VertexC->m_Coordinate[1] - VertexB->m_Coordinate[1]) +
	 VertexB->m_Coordinate[0] * (VertexA->m_Coordinate[1] - VertexC->m_Coordinate[1]) +
	 VertexC->m_Coordinate[0] * (VertexB->m_Coordinate[1] - VertexA->m_Coordinate[1])
	 ) / denom;

 Result->m_Coordinate[0] = VertexA->m_Coordinate[0] + s * (VertexB->m_Coordinate[0] - VertexA->m_Coordinate[0]);
 Result->m_Coordinate[1] = VertexA->m_Coordinate[1] + s * (VertexB->m_Coordinate[1] - VertexA->m_Coordinate[1]);
 Result->m_Coordinate[2] = VertexA->m_Coordinate[2] + s * (VertexB->m_Coordinate[2] - VertexA->m_Coordinate[2]);

 if ((0.0 <= s) && (s <= 1.0) && (0.0 <= t) && (t <= 1.0)
	 )
	 return true;
 else
	 return false;
}

double CPredicates::Distance2(CVertex * Vertex1, CVertex * Vertex2)
{
 double DeltaX = Vertex2->m_Coordinate[0] - Vertex1->m_Coordinate[0];
 double DeltaY = Vertex2->m_Coordinate[1] - Vertex1->m_Coordinate[1];

 double Distance = DeltaX * DeltaX + DeltaY * DeltaY;
	
 return Distance;
}

bool CPredicates::EqualSegments(CVertex *  Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex4)
{
 if (Distance2(Vertex1, Vertex3) < 0.00001 && Distance2(Vertex2, Vertex4) < 0.00001)
	return true; 

 if (Distance2(Vertex1, Vertex4) < 0.00001 && Distance2(Vertex2, Vertex3) < 0.00001)
	return true; 

 return false;
}