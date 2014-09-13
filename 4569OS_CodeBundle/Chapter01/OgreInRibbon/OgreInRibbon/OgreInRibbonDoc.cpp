// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// OgreInRibbonDoc.cpp : implementation of the COgreInRibbonDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OgreInRibbon.h"
#endif

#include "OgreInRibbonDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COgreInRibbonDoc

IMPLEMENT_DYNCREATE(COgreInRibbonDoc, CDocument)

BEGIN_MESSAGE_MAP(COgreInRibbonDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COgreInRibbonDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IOgreInRibbon to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {23165206-081A-45B6-B638-ABCC106D38B4}
static const IID IID_IOgreInRibbon =
{ 0x23165206, 0x81A, 0x45B6, { 0xB6, 0x38, 0xAB, 0xCC, 0x10, 0x6D, 0x38, 0xB4 } };

BEGIN_INTERFACE_MAP(COgreInRibbonDoc, CDocument)
	INTERFACE_PART(COgreInRibbonDoc, IID_IOgreInRibbon, Dispatch)
END_INTERFACE_MAP()


// COgreInRibbonDoc construction/destruction

COgreInRibbonDoc::COgreInRibbonDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

COgreInRibbonDoc::~COgreInRibbonDoc()
{
	AfxOleUnlockApp();
}

BOOL COgreInRibbonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	UpdateAllViews(NULL);

	return TRUE;
}




// COgreInRibbonDoc serialization

void COgreInRibbonDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void COgreInRibbonDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void COgreInRibbonDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COgreInRibbonDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COgreInRibbonDoc diagnostics

#ifdef _DEBUG
void COgreInRibbonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COgreInRibbonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COgreInRibbonDoc commands
