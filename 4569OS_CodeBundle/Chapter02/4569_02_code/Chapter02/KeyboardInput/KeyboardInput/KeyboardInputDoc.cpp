
// KeyboardInputDoc.cpp : implementation of the CKeyboardInputDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KeyboardInput.h"
#endif

#include "KeyboardInputDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKeyboardInputDoc

IMPLEMENT_DYNCREATE(CKeyboardInputDoc, CDocument)

BEGIN_MESSAGE_MAP(CKeyboardInputDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CKeyboardInputDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IKeyboardInput to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {DB130AF4-AE74-4E06-8933-9DF7861C4910}
static const IID IID_IKeyboardInput =
{ 0xDB130AF4, 0xAE74, 0x4E06, { 0x89, 0x33, 0x9D, 0xF7, 0x86, 0x1C, 0x49, 0x10 } };

BEGIN_INTERFACE_MAP(CKeyboardInputDoc, CDocument)
	INTERFACE_PART(CKeyboardInputDoc, IID_IKeyboardInput, Dispatch)
END_INTERFACE_MAP()


// CKeyboardInputDoc construction/destruction

CKeyboardInputDoc::CKeyboardInputDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CKeyboardInputDoc::~CKeyboardInputDoc()
{
	AfxOleUnlockApp();
}

BOOL CKeyboardInputDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CKeyboardInputDoc serialization

void CKeyboardInputDoc::Serialize(CArchive& ar)
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
void CKeyboardInputDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CKeyboardInputDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKeyboardInputDoc::SetSearchContent(const CString& value)
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

// CKeyboardInputDoc diagnostics

#ifdef _DEBUG
void CKeyboardInputDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKeyboardInputDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKeyboardInputDoc commands
