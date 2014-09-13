
// StaticGrammarDoc.cpp : implementation of the CStaticGrammarDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "StaticGrammar.h"
#endif

#include "StaticGrammarDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStaticGrammarDoc

IMPLEMENT_DYNCREATE(CStaticGrammarDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaticGrammarDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CStaticGrammarDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IStaticGrammar to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {7B303943-8E53-4E9C-AAC1-09D5C7F9214F}
static const IID IID_IStaticGrammar =
{ 0x7B303943, 0x8E53, 0x4E9C, { 0xAA, 0xC1, 0x9, 0xD5, 0xC7, 0xF9, 0x21, 0x4F } };

BEGIN_INTERFACE_MAP(CStaticGrammarDoc, CDocument)
	INTERFACE_PART(CStaticGrammarDoc, IID_IStaticGrammar, Dispatch)
END_INTERFACE_MAP()


// CStaticGrammarDoc construction/destruction

CStaticGrammarDoc::CStaticGrammarDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CStaticGrammarDoc::~CStaticGrammarDoc()
{
	AfxOleUnlockApp();
}

BOOL CStaticGrammarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	UpdateAllViews(NULL);
	return TRUE;
}




// CStaticGrammarDoc serialization

void CStaticGrammarDoc::Serialize(CArchive& ar)
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
void CStaticGrammarDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CStaticGrammarDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CStaticGrammarDoc::SetSearchContent(const CString& value)
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

// CStaticGrammarDoc diagnostics

#ifdef _DEBUG
void CStaticGrammarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaticGrammarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStaticGrammarDoc commands
