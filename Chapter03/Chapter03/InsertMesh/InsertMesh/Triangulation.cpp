// Triangulation.cpp : implementation file
//

#include "stdafx.h"
#include "Triangulation.h"
#include "Predicates.h"

// CTriangulation
#include <vector>
#include <cmath>

using namespace std;

CTriangulation::CTriangulation()
{
	m_VertexNumber = 0;
}

CTriangulation::~CTriangulation()
{
}


// CTriangulation member functions
int CTriangulation::VertexSerialize(CString Path, enum CArchive::Mode Mode)
{
 CFile File;
 
 switch (Mode)
 {
 case CArchive::load:

 if (!File.Open(Path, CFile::modeRead))
   return -1;
 
 break;
	 
 case CArchive::store:

 if (!File.Open(Path, CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite))
   return -1;

 break;
 }

 CArchive Archive(&File, Mode);

 m_VertexCollection.Serialize(Archive, this);

 Archive.Close();
 File.SeekToBegin();
 File.Close();

 return 0;
}

void CTriangulation::Shuffle(void)
{
 INT_PTR Size = m_VertexCollection.m_Collection.GetSize();
 CVertex * Vertex = NULL;
 srand((unsigned)time(NULL));
 
 for (long int OldIndex = 4; OldIndex < Size; OldIndex++)
 {
  long int NewIndex = (long int)(rand() / (RAND_MAX + 1.0) * Size + 4);
  
  if (OldIndex != NewIndex && NewIndex > 3 && NewIndex < Size)
  {
   Vertex = (CVertex *)m_VertexCollection.m_Collection[OldIndex];
   m_VertexCollection.m_Collection[OldIndex] = m_VertexCollection.m_Collection[NewIndex];
   m_VertexCollection.m_Collection[NewIndex] = Vertex;

   Vertex = (CVertex *)m_VertexCollection.m_Collection[OldIndex];
   Vertex->m_Index = OldIndex;
   Vertex = (CVertex *)m_VertexCollection.m_Collection[NewIndex];
   Vertex->m_Index = NewIndex;
  }
 }  
}

void CTriangulation::DelaunayIt(void)
{
 CVertex *Vertex = NULL;
 bool IsDelaunay = true;
 m_IncrementalBuild = false;

 Shuffle();

 CVertex *Vertex1 = (CVertex *)m_VertexCollection.m_Collection[0];
 CVertex *Vertex2 = (CVertex *)m_VertexCollection.m_Collection[1];
 CVertex *Vertex3 = (CVertex *)m_VertexCollection.m_Collection[2];
 CVertex *Vertex4 = (CVertex *)m_VertexCollection.m_Collection[3];
 
 FrameBuilder();
 DelaunayStart();

 CVertex * VertexMin = (CVertex *)m_VertexCollection.m_Collection[0];
 CVertex * VertexMax = (CVertex *)m_VertexCollection.m_Collection[2];

 for (long int Index = 5; Index < m_VertexCollection.m_Collection.GetCount(); Index++)
 {
  Vertex = (CVertex *)m_VertexCollection.m_Collection[Index];
  VertexInsert(Vertex, false);
 }

 for (int i = 0; i < 100; i++)
 {
  IsDelaunay = ImproveDelaunay();

  if (IsDelaunay)
	break;
 }
}

void CTriangulation::FrameBuilder(void)
{
 CEdge * Border = NULL;

 Border = (CEdge *)new CEdge((CVertex *)m_VertexCollection.m_Collection[0], (CVertex *)m_VertexCollection.m_Collection[1]);
 Border->m_Swappable = false;
 m_EdgeCollection.m_Collection.Add(Border);

 Border = (CEdge *)new CEdge((CVertex *)m_VertexCollection.m_Collection[1], (CVertex *)m_VertexCollection.m_Collection[2]);
 Border->m_Swappable = false;
 m_EdgeCollection.m_Collection.Add(Border);

 Border = (CEdge *)new CEdge((CVertex *)m_VertexCollection.m_Collection[2], (CVertex *)m_VertexCollection.m_Collection[3]);
 Border->m_Swappable = false;
 m_EdgeCollection.m_Collection.Add(Border);

 Border = (CEdge *)new CEdge((CVertex *)m_VertexCollection.m_Collection[3], (CVertex *)m_VertexCollection.m_Collection[0]);
 Border->m_Swappable = false;
 m_EdgeCollection.m_Collection.Add(Border);
}

void CTriangulation::DelaunayStart(void)
{
 CEdge *Diagonal1 = NULL;
 CEdge *Diagonal2 = NULL;
 CEdge *Diagonal3 = NULL;
 CEdge *Diagonal4 = NULL;

 CEdge *Symmetry1 = NULL;
 CEdge *Symmetry2 = NULL;
 CEdge *Symmetry3 = NULL;
 CEdge *Symmetry4 = NULL;

 CEdge *Border1 = NULL;
 CEdge *Border2 = NULL;
 CEdge *Border3 = NULL;
 CEdge *Border4 = NULL;

 Diagonal1 = new CEdge((CVertex *)m_VertexCollection.m_Collection[1], (CVertex *)m_VertexCollection.m_Collection[4]);
 m_EdgeCollection.m_Collection.Add(Diagonal1);
 Diagonal2 = new CEdge((CVertex *)m_VertexCollection.m_Collection[2], (CVertex *)m_VertexCollection.m_Collection[4]);
 m_EdgeCollection.m_Collection.Add(Diagonal2);
 Diagonal3 = new CEdge((CVertex *)m_VertexCollection.m_Collection[3], (CVertex *)m_VertexCollection.m_Collection[4]);
 m_EdgeCollection.m_Collection.Add(Diagonal3);
 Diagonal4 = new CEdge((CVertex *)m_VertexCollection.m_Collection[0], (CVertex *)m_VertexCollection.m_Collection[4]);
 m_EdgeCollection.m_Collection.Add(Diagonal4);

 Symmetry1 = new CEdge((CVertex *)m_VertexCollection.m_Collection[4], (CVertex *)m_VertexCollection.m_Collection[1]);
 m_EdgeCollection.m_Collection.Add(Symmetry1);
 Symmetry2 = new CEdge((CVertex *)m_VertexCollection.m_Collection[4], (CVertex *)m_VertexCollection.m_Collection[2]);
 m_EdgeCollection.m_Collection.Add(Symmetry2);
 Symmetry3 = new CEdge((CVertex *)m_VertexCollection.m_Collection[4], (CVertex *)m_VertexCollection.m_Collection[3]);
 m_EdgeCollection.m_Collection.Add(Symmetry3);
 Symmetry4 = new CEdge((CVertex *)m_VertexCollection.m_Collection[4], (CVertex *)m_VertexCollection.m_Collection[0]);
 m_EdgeCollection.m_Collection.Add(Symmetry4);

 Diagonal1->m_Symmetry = Symmetry1;
 Diagonal2->m_Symmetry = Symmetry2;
 Diagonal3->m_Symmetry = Symmetry3;
 Diagonal4->m_Symmetry = Symmetry4;

 Symmetry1->m_Symmetry = Diagonal1;
 Symmetry2->m_Symmetry = Diagonal2;
 Symmetry3->m_Symmetry = Diagonal3;
 Symmetry4->m_Symmetry = Diagonal4;

 Border1 = (CEdge *)m_EdgeCollection.m_Collection[0];
 Border2 = (CEdge *)m_EdgeCollection.m_Collection[1];
 Border3 = (CEdge *)m_EdgeCollection.m_Collection[2];
 Border4 = (CEdge *)m_EdgeCollection.m_Collection[3];

 Border1->m_Left = Diagonal1;
 Border2->m_Left = Diagonal2;
 Border3->m_Left = Diagonal3;
 Border4->m_Left = Diagonal4;

 Diagonal1->m_Left = Symmetry4;
 Diagonal2->m_Left = Symmetry1;
 Diagonal3->m_Left = Symmetry2;
 Diagonal4->m_Left = Symmetry3;

 Symmetry1->m_Left = Border2;
 Symmetry2->m_Left = Border3;
 Symmetry3->m_Left = Border4;
 Symmetry4->m_Left = Border1;
}

CEdge *CTriangulation::VertexInsert(CVertex * Vertex, bool Force)
{
 CEdge *Triangle1 = NULL;
 CEdge *Triangle2 = NULL;
 CEdge *Triangle3 = NULL;

 bool OnEdge = false;
 bool IsDelaunay = false;

 Triangle1 = LocateTriangle(Vertex, OnEdge);

 if (Triangle1 == NULL || OnEdge)
   return NULL;

 Triangle2 = Triangle1->m_Left; 
 Triangle3 = Triangle2->m_Left;

 CEdge *Edge1 = new CEdge(Triangle3->m_Destination, Vertex);
 
 if (Edge1 == NULL)
 {
  return NULL;
 }

 m_EdgeCollection.m_Collection.Add(Edge1);
 
 CEdge *Edge2 = new CEdge(Triangle1->m_Destination, Vertex);
  
 if (Edge2 == NULL)
 {
  return NULL;
 }

 m_EdgeCollection.m_Collection.Add(Edge2);
 
 CEdge *Edge3 = new CEdge(Triangle2->m_Destination, Vertex);
  
 if (Edge3 == NULL)
 {
  return NULL;
 }

 m_EdgeCollection.m_Collection.Add(Edge3);
 
 CEdge *Sym1 = new CEdge(Vertex, Triangle3->m_Destination);
 
 if (Sym1 == NULL)
 {
  return NULL;
 }

 m_EdgeCollection.m_Collection.Add(Sym1);
 CEdge *Sym2 = new CEdge(Vertex, Triangle1->m_Destination);
 
 if (Sym2 == NULL)
 {
  return NULL;
 }
 
 m_EdgeCollection.m_Collection.Add(Sym2);
 CEdge *Sym3 = new CEdge(Vertex, Triangle2->m_Destination);
 
 if (Sym3 == NULL)
 {
  return NULL;
 }
 
 m_EdgeCollection.m_Collection.Add(Sym3);
 
 Edge1->m_Symmetry = Sym1;
 Edge2->m_Symmetry = Sym2;
 Edge3->m_Symmetry = Sym3;

 Sym1->m_Symmetry = Edge1;
 Sym2->m_Symmetry = Edge2;
 Sym3->m_Symmetry = Edge3;

 Triangle1->m_Left = Edge2;
 Triangle2->m_Left = Edge3;
 Triangle3->m_Left = Edge1;

 Edge1->m_Left = Sym3;
 Edge2->m_Left = Sym1;
 Edge3->m_Left = Sym2;

 Sym1->m_Left = Triangle1;
 Sym2->m_Left = Triangle2;
 Sym3->m_Left = Triangle3;

 if (Edge1->m_Destination == m_VertexCollection.m_Collection[0] || Edge1->m_Destination == m_VertexCollection.m_Collection[1] ||
	  Edge1->m_Destination == m_VertexCollection.m_Collection[2] || Edge1->m_Destination == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge1->m_Artificial = true;
  Edge1->m_Symmetry->m_Artificial = true;
 }

 if (Edge1->m_Origin == m_VertexCollection.m_Collection[0] || Edge1->m_Origin == m_VertexCollection.m_Collection[1] ||
	  Edge1->m_Origin == m_VertexCollection.m_Collection[2] || Edge1->m_Origin == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge1->m_Artificial = true;
  Edge1->m_Symmetry->m_Artificial = true;
 }

 if (Edge2->m_Destination == m_VertexCollection.m_Collection[0] || Edge2->m_Destination == m_VertexCollection.m_Collection[1] ||
	  Edge2->m_Destination == m_VertexCollection.m_Collection[2] || Edge2->m_Destination == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge2->m_Artificial = true;
  Edge2->m_Symmetry->m_Artificial = true;
 }

 if (Edge2->m_Origin == m_VertexCollection.m_Collection[0] || Edge2->m_Origin == m_VertexCollection.m_Collection[1] ||
	  Edge2->m_Origin == m_VertexCollection.m_Collection[2] || Edge2->m_Origin == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge2->m_Artificial = true;
  Edge2->m_Symmetry->m_Artificial = true;
 }

 if (Edge3->m_Origin == m_VertexCollection.m_Collection[0] || Edge3->m_Origin == m_VertexCollection.m_Collection[1] ||
	  Edge3->m_Origin == m_VertexCollection.m_Collection[2] || Edge3->m_Origin == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge3->m_Artificial = true;
  Edge3->m_Symmetry->m_Artificial = true;
 }

 if (Edge3->m_Destination == m_VertexCollection.m_Collection[0] || Edge3->m_Destination == m_VertexCollection.m_Collection[1] ||
	  Edge3->m_Destination == m_VertexCollection.m_Collection[2] || Edge3->m_Destination == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge3->m_Artificial = true;
  Edge3->m_Symmetry->m_Artificial = true;
 }

 // Check for Delaunay

 CEdge *Edge = Triangle3;

 while (true)
 {
  CEdge *Temp = Edge->m_Symmetry;

  if (Temp == NULL)
	 break;
  
  Temp = Temp->m_Left;

  if (Temp == NULL)
	 break;

  if (Force)
	  IsDelaunay = true;
  else
	  IsDelaunay = Edge->IsDelaunay(Vertex);

  if (IsDelaunay)
  {
   if (Edge->m_Origin == Triangle1->m_Origin)
	  break;
 
   Edge = Edge->m_Left;

   if (Edge == NULL)
	   break;
   
   Edge = Edge->m_Left;

   if (Edge == NULL)
	   break;

   Edge = Edge->m_Symmetry;

   Edge = Edge->m_Left;

   if (Edge == NULL)
	   break;
   
   Edge = Edge->m_Left;

   if (Edge == NULL)
	   break;
  }
  else
  {
   Edge->Swap();
 ///////////////////////////////////////  
   Edge->m_Artificial = false;
   Edge->m_Symmetry->m_Artificial = false;

   if (Edge->m_Destination == m_VertexCollection.m_Collection[0] || Edge->m_Destination == m_VertexCollection.m_Collection[1] ||
	  Edge->m_Destination == m_VertexCollection.m_Collection[2] || Edge->m_Destination == m_VertexCollection.m_Collection[3] 
	 )
   {
    Edge->m_Artificial = true;
	Edge->m_Symmetry->m_Artificial = true;
   }

 if (Edge->m_Origin == m_VertexCollection.m_Collection[0] || Edge->m_Origin == m_VertexCollection.m_Collection[1] ||
	  Edge->m_Origin == m_VertexCollection.m_Collection[2] || Edge->m_Origin == m_VertexCollection.m_Collection[3] 
	 )
 {
  Edge->m_Artificial = true;
  Edge->m_Symmetry->m_Artificial = true;
 }
///////////////////////////////////////
   Edge = Edge->m_Symmetry->m_Left;
   
   if (Edge == NULL)
	   break;
  }
 }
 
 return Edge1;
}


CEdge * CTriangulation::LocateTriangle(CVertex * Vertex, bool &OnEdge)
{
 long EdgeIndex = 0;
 CEdge *Left = NULL;

 CEdge *Edge = (CEdge *)m_EdgeCollection.m_Collection[EdgeIndex];
 Edge->m_Visitor = Vertex->m_Index;

 VisitClear(Vertex->m_Index);

 while (true)
 {
  if (
	   NearEqual(Edge->m_Destination->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	   NearEqual(Edge->m_Destination->m_Coordinate[1], Vertex->m_Coordinate[1]) 
	  )
	  {
       OnEdge = true;
       return Edge;
	  }

  if (
	   NearEqual(Edge->m_Origin->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	   NearEqual(Edge->m_Origin->m_Coordinate[1], Vertex->m_Coordinate[1])
	  )
	  {
	   OnEdge = true;
       return Edge;
	  }

  if (Edge->RightOn(Vertex))
  {
   Edge = Edge->m_Symmetry;

   if (Edge == NULL)
	 return NULL;

   if (Edge->m_Visitor == Vertex->m_Index && Vertex->m_Index != 0)
     return NULL;

   continue;
  }

  Edge = Edge->m_Left;

  if (Edge == NULL)
	 return NULL;

  if (Edge->m_Visitor == Vertex->m_Index && Vertex->m_Index != 0)
    return NULL;


  if (
	   NearEqual(Edge->m_Destination->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	   NearEqual(Edge->m_Destination->m_Coordinate[1], Vertex->m_Coordinate[1]) 
	  )
	  {
         OnEdge = true;
		 return Edge;
	  }
  if (
	   NearEqual(Edge->m_Origin->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	   NearEqual(Edge->m_Origin->m_Coordinate[1], Vertex->m_Coordinate[1])
	  )
	  {
	   OnEdge = true;
       return Edge;
	  }
  
	  if (Edge->RightOn(Vertex))
  {
   Edge = Edge->m_Symmetry;

   if (Edge == NULL)
	 return NULL;

 if (Edge->m_Visitor == Vertex->m_Index && Vertex->m_Index != 0)
   return NULL;

   continue;
  }
   
  Edge = Edge->m_Left;

  if (Edge == NULL)
	 return NULL;

 if (Edge->m_Visitor == Vertex->m_Index && Vertex->m_Index != 0)
   return NULL;

  if (
	   NearEqual(Edge->m_Destination->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	   NearEqual(Edge->m_Destination->m_Coordinate[1], Vertex->m_Coordinate[1]) 
	  )
	  {
       OnEdge = true;
       return Edge;
	  }

  if (
	   NearEqual(Edge->m_Origin->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	   NearEqual(Edge->m_Origin->m_Coordinate[1], Vertex->m_Coordinate[1])
	  )
	  {
	   OnEdge = true;
       return Edge;
	  }

  if (Edge->RightOn(Vertex))
  {
   Edge = Edge->m_Symmetry;

   if (Edge == NULL)
	 return NULL;

   if (Edge->m_Visitor == Vertex->m_Index && Vertex->m_Index != 0)
     return NULL;

   Edge->m_Visitor = Vertex->m_Index;

   continue;
  }

  Left = Edge->m_Left;
  
  if (Left != NULL)
  {
   if (
       NearEqual(Left->m_Destination->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
       NearEqual(Left->m_Destination->m_Coordinate[1], Vertex->m_Coordinate[1]) 
	  )
  
   OnEdge = true;
   Edge = Left;
  }

  return Edge;
 }
}


void CTriangulation::ClearPrinted(void)
{
 for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  CEdge *Edge = (CEdge*) m_EdgeCollection.m_Collection[EdgeIndex];
  Edge->m_Printed = false;
 }
}


void CTriangulation::Terminator(void)
{
 CEdge * Edge = NULL;
 double Ratio = 0.0;
 double Area = 0.0;
 double RatioBound = sqrt(2.0);
 double AreaBound = 0.0;

 for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  Edge = (CEdge *) m_EdgeCollection.m_Collection[EdgeIndex];

  if (Edge == NULL)
	  continue;
  if (Edge->m_Left == NULL)
	  continue;
//Area
  Ratio = CPredicates::TriangleRatio(Edge->m_Origin, Edge->m_Destination, Edge->m_Left->m_Destination);
  if (Ratio > RatioBound)
  {
    Edge->m_Artificial = true;
  }
//  if (Edge->m_Artificial)
//  {
//   do
//   {
//   }
//   while 
//  }
 } 
}

void CTriangulation::TriangleSerialize(CString Path)
{
 CFile File;
 CString Record;
 long DeltaIndex = 0;
  
 if (!m_IncrementalBuild)
 {
  m_TriangleNumber = 0;
 }
 else
 {
  DeltaIndex = this->m_VertexNumber;
 }

 if (m_IncrementalBuild)
 {
  if (!File.Open(Path, CFile::shareDenyNone | CFile::modeWrite))
    return;
  File.SeekToEnd();
 }
 else
 {
  if (!File.Open(Path, CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite))
   return;
 }
 
 CArchive Archive(&File, CArchive::store);

 CEdge *Edge1 = NULL;
 CEdge *Edge2 = NULL;
 CEdge *Edge3 = NULL;

 CVertex Verteces[3];
 CVertex *Vertex1 = NULL;
 CVertex *Vertex2 = NULL;
 CVertex *Vertex3 = NULL;

 int InsideCounter;

 ClearPrinted();

 int VertexIndex = 0;
 int FaceIndex = 0;

 for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  Edge1 = (CEdge *) m_EdgeCollection.m_Collection[EdgeIndex];
 
  if (Edge1 == NULL)
	  continue;

  if (Edge1->m_Printed)
     continue;

  Vertex1 = Edge1->m_Origin;
  
  if (Vertex1->m_Index < 4)
  {
   continue;
  }

  Edge2 = Edge1->m_Left;
  
  if (Edge2 == NULL)
    continue;

  if (Edge2->m_Printed)
     continue;

   Vertex2 = Edge2->m_Origin;

  if (Vertex2->m_Index < 4)
   {
    continue;
   }

   Edge3 = Edge2->m_Left;

   if (Edge3 == NULL)
    continue;

   if (Edge3->m_Printed)
     continue;

   Vertex3 = Edge3->m_Origin;

  if (Vertex3->m_Index < 4)
   {
	   continue;
   }

   InsideCounter = 0;

   if (Edge1->m_Inside)
   {
	    continue;
   }

  if (Edge2->m_Inside)
  {
	   continue;
  }

   if (Edge3->m_Inside)
   {
	   continue;
   }

   CVertex::VertexType Type1 = Vertex1->m_VertexType;
   CVertex::VertexType Type2 = Vertex2->m_VertexType;
   CVertex::VertexType Type3 = Vertex3->m_VertexType;

   if (Type1 == CVertex::Shoulder || Type1 == CVertex::Hole)
  {
    continue;
  }

   if (Type2 == CVertex::Shoulder || Type2 == CVertex::Hole)
  {
    continue;
  }

   if (Type3 == CVertex::Shoulder || Type3 == CVertex::Hole)
  {
    continue;
  }

   Edge1->m_Printed = true;
   Edge2->m_Printed = true;
   Edge3->m_Printed = true;
 
   Record.Format(_T(" %ld %ld %ld"),Vertex1->m_Index - 4 + (DeltaIndex), Vertex2->m_Index - 4 + (DeltaIndex), Vertex3->m_Index - 4 + (DeltaIndex));
   Archive.WriteString(Record);
   Archive.WriteString(_T("\n"));
   m_TriangleNumber++;
}
 
 Archive.Close();
 File.SeekToBegin();
 File.Close();
}

void CTriangulation::Eraser(void)
{
	CVertex *Vertex = NULL;

	for (long VertexIndex = 0; VertexIndex < m_VertexCollection.m_Collection.GetCount(); VertexIndex++)
	{
     Vertex = (CVertex*)m_VertexCollection.m_Collection[VertexIndex];
	 
	 if (Vertex != NULL)
	   delete Vertex;
	}

	m_VertexCollection.m_Collection.RemoveAll();

	for (long EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
	{
     CEdge *Edge = (CEdge*)m_EdgeCollection.m_Collection[EdgeIndex];
	 
	 if (Edge != NULL)
	   delete Edge;
	}

	m_EdgeCollection.m_Collection.RemoveAll();
}


//void CTriangulation::NavigationIn(CEdge *Edge)
//{
//	CEdge *Symmetry = Edge->m_Symmetry;
//
//	if (Symmetry == NULL)
//      return;
//
//	if (Symmetry->m_Inside)
//      return;
//
//	if (Symmetry->m_Outside)
//      return;
//
//	if (Symmetry->m_Intersected)
//      return;
//
//	Symmetry->m_Inside = true;
//
//	CEdge *Left = Symmetry->m_Left;
//
//    if (Left->m_Inside)
//      return;
//
//	if (Left->m_Outside)
//      return;
//
//	if (Left->m_Intersected)
//      return;
//
//	if (!Left->m_Inside)
//	{
//     Left->m_Inside = true;
//     NavigationIn(Left);
//	}
//	
//	Left = Left->m_Left;
//
//	if (Left->m_Inside)
//      return;
//
//	if (Left->m_Outside)
//      return;
//
//	if (Left->m_Intersected)
//      return;
//
//	if (!Left->m_Inside)
//	{
//     Left->m_Inside = true;
//     NavigationIn(Left);
//	}
//}

void CTriangulation::AllIncluded(void)
{
	for (long EdgeIndex = 4; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
	{
     CEdge *Edge = (CEdge*)m_EdgeCollection.m_Collection[EdgeIndex];
	 Edge->m_Inside = false;
	}
}


void CTriangulation::VertexSwap(CObject ** Vertex1, CObject ** Vertex2)
{
 CObject *Object = NULL;

 Object = *Vertex1;
 *Vertex1 = *Vertex2;
 *Vertex2 = Object;
}

void CTriangulation::VertexSwap(CObject * Vertex1, CObject * Vertex2)
{
 CObject *Object = NULL;

 Object = Vertex1;
 Vertex1 = Vertex2;
 Vertex2 = Object;
}

bool CTriangulation::ImproveDelaunay(void)
{
 CEdge * Edge = NULL;
 bool IsDelaunay = true;

 for (long int EdgeIndex = 4; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  Edge = (CEdge *) m_EdgeCollection.m_Collection[EdgeIndex];

  if (Edge == NULL)
	  continue;

  if (Edge->m_Left == NULL)
	  continue;

  if (Edge->m_Symmetry == NULL)
	  continue;

  if (!Edge->IsDelaunay())
  {
	 Edge->Swap();
     IsDelaunay = false;
  }
 }

 return IsDelaunay;
}

bool CTriangulation::RightOn(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3)
{
 return CPredicates::CounterClockWise(Vertex1, Vertex3, Vertex2);
}

bool CTriangulation::NearEqual(double Number1, double Number2)
{
 if (fabs(Number1 - Number2) < 0.0001)
    return true;
 else
	return false;
}

bool CTriangulation::LeftOn(CVertex * Vertex1 , CVertex * Vertex2 , CVertex * Vertex3)
{
 return CPredicates::CounterClockWise(Vertex1, Vertex2, Vertex3);
}


void CTriangulation::FixVertex(CEdge* Edge, CVertex* Vertex)
{
 CVertex* V1 = NULL;
 CVertex* V2 = NULL;
 CVertex* V3 = NULL;

 V1 = Edge->m_Origin;
 V2 = Edge->m_Destination;
 V3 = Edge->m_Left->m_Destination;

 if (NearEqual(V1->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	 NearEqual(V1->m_Coordinate[1], Vertex->m_Coordinate[1])
	 )
 {
	 Vertex->m_Coordinate[0] += 0.2;
	 Vertex->m_Coordinate[1] += 0.2;
 }
 
 if (NearEqual(V2->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	 NearEqual(V2->m_Coordinate[1], Vertex->m_Coordinate[1])
	 )
 {
	 Vertex->m_Coordinate[0] += 0.2;
	 Vertex->m_Coordinate[1] += 0.2;
 }

 if (NearEqual(V3->m_Coordinate[0], Vertex->m_Coordinate[0]) &&
	 NearEqual(V3->m_Coordinate[1], Vertex->m_Coordinate[1])
	 )
 {
	 Vertex->m_Coordinate[0] += 0.2;
	 Vertex->m_Coordinate[1] += 0.2;
 }
}

//void CTriangulation::CleanIntersect(void)
//{
// for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
// {
//  CEdge *Edge = (CEdge*) m_EdgeCollection.m_Collection[EdgeIndex];
//  
//  if (Edge->m_Intersected)
//  {
//   Edge->m_Inside = false;
//  }
// }
//}

void CTriangulation::FixInside(void)
{
 for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  CEdge *Edge = (CEdge*) m_EdgeCollection.m_Collection[EdgeIndex];
  
  if (Edge->m_Inside)
  {
   Edge->m_Left->m_Inside = true;
   Edge->m_Left->m_Left->m_Inside = true;
  }
  
//  if (Edge->m_Intersected)
//  {
//   Edge->m_Intersected = false;
//   Edge->m_Inside = true;
//  }
 }
}


void CTriangulation::VisitClear(void)
{
 for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  CEdge * Edge = (CEdge *) m_EdgeCollection.m_Collection[EdgeIndex];
  Edge->m_Visitor = 0;
 }
}

void CTriangulation::VisitClear(long Visitor)
{
 for (long int EdgeIndex = 0; EdgeIndex < m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  CEdge * Edge = (CEdge *) m_EdgeCollection.m_Collection[EdgeIndex];
  
  if (Edge->m_Visitor == Visitor)
    Edge->m_Visitor = 0;
 }
}

CVertex Center;//circle center

CVertex& CTriangulation::GetCircleCenter(CEdge * Edge, double Radius, enum CircleSide Side)
{
	double x1 = Edge->m_Origin->m_Coordinate[0];
	double x2 = Edge->m_Destination->m_Coordinate[0];
	double xm; //middle of edge

	double y1 = Edge->m_Origin->m_Coordinate[1];
	double y2 = Edge->m_Destination->m_Coordinate[1];
	double ym; //middle of edge

	xm = (x1 + x2) / 2.0;
	ym = (y1 + y2) / 2.0;

	double Length = sqrt(CPredicates::Distance2(Edge->m_Origin, Edge->m_Destination));
	double Distance2Move = sqrt(fabs(pow(Radius, 2.0) - pow(Length / 2.0, 2.0)));

	if (Side == RightSide)
	{
		Center.m_Coordinate[0] = xm - Distance2Move * (y1 - y2) / Length;
		Center.m_Coordinate[1] = ym - Distance2Move * (x2 - x1) / Length;
	}
	else
	{
		Center.m_Coordinate[0] = xm + Distance2Move * (y1 - y2) / Length;
		Center.m_Coordinate[1] = ym + Distance2Move * (x2 - x1) / Length;
	}
	
	return Center;
}


double CTriangulation::TriangleInterpolate(CVertex * Vertex1, CVertex * Vertex2, CVertex * Vertex3, CVertex * Vertex)
{
	double Z = 0.0;

	double A1 = CPredicates::TriangleArea(Vertex2, Vertex3, Vertex);
	double A2 = CPredicates::TriangleArea(Vertex3, Vertex1, Vertex);
	double A3 = CPredicates::TriangleArea(Vertex1, Vertex2, Vertex);

	Z = (Vertex1->m_Coordinate[2] * A1 + Vertex2->m_Coordinate[2] * A2 + Vertex3->m_Coordinate[2] * A3) / (A1 + A2 +A3);

	if (Z < Vertex1->m_Coordinate[2] && Z < Vertex2->m_Coordinate[2] && Z < Vertex3->m_Coordinate[2])
	{
		Z = Vertex1->m_Coordinate[2];
	}

	if (Z > Vertex1->m_Coordinate[2] && Z > Vertex2->m_Coordinate[2] && Z > Vertex3->m_Coordinate[2])
	{
		Z = Vertex1->m_Coordinate[2];
	}

	return Z;
}

bool CTriangulation::OnLevel(CEdge * Edge, CVertex * Base, CVertex * Vertex)
{
	double Dist1;
	double Dist2;

	Dist1 = Edge->m_Origin->m_Coordinate[2] - Base->m_Coordinate[2];

	if (Dist1 == 0.0)
	{
		Vertex->m_Coordinate[0] = Edge->m_Origin->m_Coordinate[0];
		Vertex->m_Coordinate[1] = Edge->m_Origin->m_Coordinate[1];
		Vertex->m_Coordinate[2] = Edge->m_Origin->m_Coordinate[2];

		return true;
	}

	Dist2 = Edge->m_Destination->m_Coordinate[2] - Base->m_Coordinate[2];

	if (Dist2 == 0.0)
	{
		Vertex->m_Coordinate[0] = Edge->m_Destination->m_Coordinate[0];
		Vertex->m_Coordinate[1] = Edge->m_Destination->m_Coordinate[1];
		Vertex->m_Coordinate[2] = Edge->m_Destination->m_Coordinate[2];

		return true;
	}

	if (Dist1 * Dist2 < (double)0.0)
	{
		Dist1 = fabs(Dist1);
		Dist2 = fabs(Dist2);

		Vertex->m_Coordinate[0] = (
                            Edge->m_Origin->m_Coordinate[0] * Dist2 +
                            Edge->m_Destination->m_Coordinate[0] * Dist1
                           ) / (Dist1 + Dist2);
		Vertex->m_Coordinate[1] = (
							Edge->m_Origin->m_Coordinate[1] * Dist2 +
                            Edge->m_Destination->m_Coordinate[1] * Dist1
                           ) / (Dist1 + Dist2);

		Vertex->m_Coordinate[2] =  Base->m_Coordinate[2];
		return true;
	}
	else
	{
		return false;
	}
}

