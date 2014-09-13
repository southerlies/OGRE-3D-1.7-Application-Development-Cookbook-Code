
// MouseInputDoc.cpp : implementation of the CMouseInputDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MouseInput.h"
#endif

#include "MouseInputDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMouseInputDoc

IMPLEMENT_DYNCREATE(CMouseInputDoc, CDocument)

BEGIN_MESSAGE_MAP(CMouseInputDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMouseInputDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IMouseInput to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {25D93ED6-3EA0-4047-BCEF-13E04D6C4943}
static const IID IID_IMouseInput =
{ 0x25D93ED6, 0x3EA0, 0x4047, { 0xBC, 0xEF, 0x13, 0xE0, 0x4D, 0x6C, 0x49, 0x43 } };

BEGIN_INTERFACE_MAP(CMouseInputDoc, CDocument)
	INTERFACE_PART(CMouseInputDoc, IID_IMouseInput, Dispatch)
END_INTERFACE_MAP()


// CMouseInputDoc construction/destruction

CMouseInputDoc::CMouseInputDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CMouseInputDoc::~CMouseInputDoc()
{
	AfxOleUnlockApp();
}

BOOL CMouseInputDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	UpdateAllViews(NULL);
	return TRUE;
}




// CMouseInputDoc serialization

void CMouseInputDoc::Serialize(CArchive& ar)
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
void CMouseInputDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CMouseInputDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMouseInputDoc::SetSearchContent(const CString& value)
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

// CMouseInputDoc diagnostics

#ifdef _DEBUG
void CMouseInputDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMouseInputDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMouseInputDoc commands
