#pragma once
#include "Vertex.h"
// CEdge command target

class CEdge : public CObject
{
public:
	CEdge();
	virtual ~CEdge();
	// Edge origin
	CVertex *m_Origin;
	// Edge destination
	CVertex *m_Destination;
	// The same edge in the opposite direction
	CEdge *m_Symmetry;
	// Next edge in the triangle, actually left in the counterclockwise direction
	CEdge *m_Left;
	
	// Yes - if we can swap this edge to improve Delaunay, no - if we can't (breakline case)/
	bool m_Swappable;
	// Artificial edge for internal use. Not to draw.
	bool m_Artificial;
	// For temporary use within drawing function
	bool m_Printed;

	CEdge(CVertex * Origin, CVertex * Destination);
	bool RightOn(CVertex *Vertex);
	bool ExactlyOn(CVertex *Vertex);
	bool IsDelaunay(CVertex * Vertex);
	void Swap(void);
	bool m_Inside;
	bool m_Intersected;
	bool m_Outside;
	bool IsDelaunay(void);
	bool m_Extracted;
	long m_Visitor;
	bool m_IsInterior;
	};


