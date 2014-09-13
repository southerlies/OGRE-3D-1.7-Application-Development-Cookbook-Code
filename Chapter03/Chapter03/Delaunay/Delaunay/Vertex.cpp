// Vertex.cpp : implementation file
//

#include "stdafx.h"
//#include "Delaunay.h"
#include "Vertex.h"


// Constructor with initialization

CVertex::CVertex()
{
 m_Coordinate[0] = 0.0;
 m_Coordinate[1] = 0.0;
 m_Coordinate[2] = 0.0;
 m_VertexType = Standalone;
 m_Cut = false;
 m_ToDelete = false;
 this->m_Index = -1;
}

CVertex::~CVertex()
{
}


// CVertex member functions

CVertex::CVertex(double x, double y, double z)
{
	m_Coordinate[0] = x;
	m_Coordinate[1] = y;
	m_Coordinate[2] = z;
	m_VertexType = Standalone;
	m_Cut = false;
	m_ToDelete = false;
	m_Index = -1;
}

CVertex::CVertex(double x, double y, double z, VertexType Type)
{
	m_Coordinate[0] = x;
	m_Coordinate[1] = y;
	m_Coordinate[2] = z;
	m_VertexType = Type;
	m_Cut = false;
	m_ToDelete = false;
	m_Index = -1;
}


//CVertex CVertex::operator+( CVertex &Vertex ) 
//{
// return CVertex(Vertex.m_Coordinate[0], Vertex.m_Coordinate[1], Vertex.m_Coordinate[2]);
//}

CVertex& CVertex::operator=(const CVertex& Vertex)
{
	m_Coordinate[0] = Vertex.m_Coordinate[0];
	m_Coordinate[1] = Vertex.m_Coordinate[1];
	m_Coordinate[2] = Vertex.m_Coordinate[2];

	m_VertexType = Vertex.m_VertexType;
	m_Cut = false;
	m_ToDelete = false;
	m_Index = -1;

	return *this;
}
