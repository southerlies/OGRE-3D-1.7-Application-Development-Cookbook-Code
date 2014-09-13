
// LinkedMovementDoc.cpp : implementation of the CLinkedMovementDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LinkedMovement.h"
#endif

#include "LinkedMovementDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLinkedMovementDoc

IMPLEMENT_DYNCREATE(CLinkedMovementDoc, CDocument)

BEGIN_MESSAGE_MAP(CLinkedMovementDoc, CDocument)
END_MESSAGE_MAP()


// CLinkedMovementDoc construction/destruction

CLinkedMovementDoc::CLinkedMovementDoc()
{
	// TODO: add one-time construction code here

}

CLinkedMovementDoc::~CLinkedMovementDoc()
{
}

BOOL CLinkedMovementDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	UpdateAllViews(NULL);
	return TRUE;
}




// CLinkedMovementDoc serialization

void CLinkedMovementDoc::Serialize(CArchive& ar)
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
void CLinkedMovementDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CLinkedMovementDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CLinkedMovementDoc::SetSearchContent(const CString& value)
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

// CLinkedMovementDoc diagnostics

#ifdef _DEBUG
void CLinkedMovementDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLinkedMovementDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLinkedMovementDoc commands
