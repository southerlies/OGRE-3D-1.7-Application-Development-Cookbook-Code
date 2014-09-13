
// OgreInMfcDoc.cpp : implementation of the COgreInMfcDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OgreInMfc.h"
#endif

#include "OgreInMfcDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COgreInMfcDoc

IMPLEMENT_DYNCREATE(COgreInMfcDoc, CDocument)

BEGIN_MESSAGE_MAP(COgreInMfcDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COgreInMfcDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IOgreInMfc to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {AD471551-0C86-4F44-A945-EA2D4B6780D8}
static const IID IID_IOgreInMfc =
{ 0xAD471551, 0xC86, 0x4F44, { 0xA9, 0x45, 0xEA, 0x2D, 0x4B, 0x67, 0x80, 0xD8 } };

BEGIN_INTERFACE_MAP(COgreInMfcDoc, CDocument)
	INTERFACE_PART(COgreInMfcDoc, IID_IOgreInMfc, Dispatch)
END_INTERFACE_MAP()


// COgreInMfcDoc construction/destruction

COgreInMfcDoc::COgreInMfcDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

COgreInMfcDoc::~COgreInMfcDoc()
{
	AfxOleUnlockApp();
}

BOOL COgreInMfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	UpdateAllViews(NULL);
	return TRUE;
}




// COgreInMfcDoc serialization

void COgreInMfcDoc::Serialize(CArchive& ar)
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
void COgreInMfcDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void COgreInMfcDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COgreInMfcDoc::SetSearchContent(const CString& value)
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

// COgreInMfcDoc diagnostics

#ifdef _DEBUG
void COgreInMfcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COgreInMfcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COgreInMfcDoc commands


BOOL COgreInMfcDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}
