// VertexCollection.cpp : implementation file
//

#include "stdafx.h"
//#include "Delaunay.h"
#include "Triangulation.h"
#include "VertexCollection.h"
#include <math.h>


// CVertexCollection

CVertexCollection::CVertexCollection()
{
 m_VertexNumber = 0;
}

CVertexCollection::~CVertexCollection()
{
}

// CVertexCollection member functions

void CVertexCollection::Serialize(CArchive& Archive, CTriangulation *Triangulation)
{
 double x = 0;
 double y = 0;
 double z = 0;
 CVertex *Vertex = NULL;
 CString sLine;

 if (Archive.IsStoring())
 {
  for (long VertexIndex = 4; VertexIndex < m_Collection.GetCount(); VertexIndex++)
  {
   Vertex = (CVertex *)m_Collection[VertexIndex];

   sLine.Format(_T(" %.3lf %.3lf %.3lf"),Vertex->m_Coordinate[0],Vertex->m_Coordinate[1], Vertex->m_Coordinate[2]);
   Archive.WriteString(sLine);
   Archive.WriteString(_T("\n"));
   m_VertexNumber++;
  }
 }
 else
 {
//Let's check file extension.

	 CFile *File = Archive.GetFile();
//	 CString FilePath = File->GetFilePath();
//	 char Ext[256];
//	 _splitpath(FilePath, NULL, NULL, NULL, Ext);

//First four verteces are verteces of bounding box/
  for (int VertexIndex = 0; VertexIndex < 4; VertexIndex++)
  {
   Vertex = new CVertex(0, 0, 0, CVertex::BoundingBox);
   Vertex->m_Index = m_Collection.Add(Vertex);
  }

  do
  {
	  if (!sLine.IsEmpty())
   {
   CString XString;
   CString YString;
   CString ZString;

   int Start = 0;

   XString = sLine.Tokenize(_T(" "), Start);
   YString = sLine.Tokenize(_T(" "), Start);
   ZString = sLine.Tokenize(_T(" "), Start);

   sscanf_s(XString,_T("%lf.2"), &x);
   sscanf_s(YString,_T("%lf.2"), &y);
   sscanf_s(ZString,_T("%lf.2"), &z);

   Vertex = new CVertex(x, y, z, CVertex::Standalone);

   Vertex->m_Index = m_Collection.Add(Vertex);

   if (m_Collection.GetCount() == 5)
   {
    m_Min.m_Coordinate[0] = x;
    m_Min.m_Coordinate[1] = y;
    m_Min.m_Coordinate[2] = z;
    m_Max.m_Coordinate[0] = x;
    m_Max.m_Coordinate[1] = y;
    m_Max.m_Coordinate[2] = z;
   }
   else
   {
//todo = min
   if (m_Min.m_Coordinate[0] > x)
     m_Min.m_Coordinate[0] = x;
   if (m_Min.m_Coordinate[1] > y)
     m_Min.m_Coordinate[1] = y;
   if (m_Min.m_Coordinate[2] > z)
     m_Min.m_Coordinate[2] = z;

   if (m_Max.m_Coordinate[0] < x)
     m_Max.m_Coordinate[0] = x;
   if (m_Max.m_Coordinate[1] < y)
     m_Max.m_Coordinate[1] = y;
   if (m_Max.m_Coordinate[2] < z)
     m_Max.m_Coordinate[2] = z;
   }
   }
	  if (m_Collection.GetSize() > 100)
		  break;
  }
  while(Archive.ReadString(sLine));
//////////////////////////////////////////

 m_DeltaX = 0.25 * fabs((m_Max.m_Coordinate[0] - m_Min.m_Coordinate[0]));
 m_DeltaY = 0.25 * fabs((m_Max.m_Coordinate[1] - m_Min.m_Coordinate[1]));
 
 m_Min.m_Coordinate[2] = (double)((long)m_Min.m_Coordinate[2]);

//////////////////////////////////////////////
  Vertex = (CVertex *)m_Collection[0];

  Vertex->m_Coordinate[0] = m_Min.m_Coordinate[0] - 2 * m_DeltaX;
  Vertex->m_Coordinate[1] = m_Min.m_Coordinate[1] - 2 * m_DeltaY;
  Vertex->m_Coordinate[2] = m_Min.m_Coordinate[2];

  Vertex = (CVertex *)m_Collection[1];

  Vertex->m_Coordinate[0] = m_Max.m_Coordinate[0] + 2 * m_DeltaX;
  Vertex->m_Coordinate[1] = m_Min.m_Coordinate[1] - 2 * m_DeltaY;
  Vertex->m_Coordinate[2] = m_Min.m_Coordinate[2];

  Vertex = (CVertex *)m_Collection[2];

  Vertex->m_Coordinate[0] = m_Max.m_Coordinate[0] + 2 * m_DeltaX;
  Vertex->m_Coordinate[1] = m_Max.m_Coordinate[1] + 2 * m_DeltaY;
  Vertex->m_Coordinate[2] = m_Min.m_Coordinate[2];

  Vertex = (CVertex *)m_Collection[3];

  Vertex->m_Coordinate[0] = m_Min.m_Coordinate[0] - 2 * m_DeltaX;
  Vertex->m_Coordinate[1] = m_Max.m_Coordinate[1] + 2 * m_DeltaY;
  Vertex->m_Coordinate[2] = m_Min.m_Coordinate[2];
 }
}