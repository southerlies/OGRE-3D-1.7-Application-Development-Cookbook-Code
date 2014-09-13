// Edge.cpp : implementation file
//

#include "stdafx.h"
//#include "Delaunay.h"
#include "Edge.h"
#include "Predicates.h"
#include <math.h>

// CEdge

CEdge::CEdge()
: m_Origin(NULL)
, m_Destination(NULL)
, m_Symmetry(NULL)
, m_Left(NULL)
, m_Swappable(true)
{
 m_Artificial = false;
 m_Printed = false;
 m_Inside = false;
 m_Intersected = false;
 m_Outside = false;
 m_Extracted = false;
 m_Visitor = 0;
 m_IsInterior = true;
}

CEdge::~CEdge()
{
}


// CEdge member functions

CEdge::CEdge(CVertex * Origin, CVertex * Destination)
: m_Symmetry(NULL)
, m_Left(NULL)
, m_Swappable(true)
{
 m_Origin = Origin;
 m_Destination = Destination;
 m_Artificial = false;
 m_Printed = false;
 m_Inside = false;
 m_Intersected = false;
 m_Outside = false;
 m_Extracted = false;
 m_Visitor = 0;
 m_IsInterior = true;
}

bool CEdge::RightOn(CVertex *Vertex)
{
 return CPredicates::CounterClockWise(Vertex, m_Destination, m_Origin);
}

bool CEdge::ExactlyOn(CVertex *Vertex)
{
 return fabs(CPredicates::TriangleArea(this->m_Origin, this->m_Destination, Vertex)) < 0.00001;
}

bool CEdge::IsDelaunay(CVertex * Vertex)
{
 CEdge *Temp = m_Symmetry->m_Left;

 if (Temp == NULL)
	return true;
 if (Temp->m_Destination == NULL)
   return true;

 if (m_Origin == NULL)
	return true;

 if (m_Destination == NULL)
	return true;

 if (RightOn(Temp->m_Destination) && 
	 CPredicates::InCircle(m_Origin, Temp->m_Destination, m_Destination, Vertex))
	return false;
 
 return true;
}

void CEdge::Swap(void)
{
 CEdge *Symmetry = m_Symmetry;
 CEdge *Border1 = m_Left;
 CEdge *Border2 = Border1->m_Left;
 CEdge *Border3 = Symmetry->m_Left;
 CEdge *Border4 = Border3->m_Left;

 m_Destination = Border1->m_Destination;
 m_Origin = Border3->m_Destination;
 Symmetry->m_Destination = Border3->m_Destination;
 Symmetry->m_Origin = Border1->m_Destination;

 m_Left = Border2;
 Symmetry->m_Left = Border4;

 Border1->m_Left = Symmetry;
 Border2->m_Left = Border3;
 Border3->m_Left = this;
 Border4->m_Left = Border1;
}

bool CEdge::IsDelaunay(void)
{
 if (CPredicates::InCircle(m_Origin, m_Symmetry->m_Left->m_Destination, m_Destination, m_Left->m_Destination))
   return false;

 return true;
}
