
// DynamicGrammarDoc.cpp : implementation of the CDynamicGrammarDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DynamicGrammar.h"
#endif

#include "DynamicGrammarDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDynamicGrammarDoc

IMPLEMENT_DYNCREATE(CDynamicGrammarDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynamicGrammarDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDynamicGrammarDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IDynamicGrammar to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {D98A9905-73B5-41D3-BB63-B922BC49B213}
static const IID IID_IDynamicGrammar =
{ 0xD98A9905, 0x73B5, 0x41D3, { 0xBB, 0x63, 0xB9, 0x22, 0xBC, 0x49, 0xB2, 0x13 } };

BEGIN_INTERFACE_MAP(CDynamicGrammarDoc, CDocument)
	INTERFACE_PART(CDynamicGrammarDoc, IID_IDynamicGrammar, Dispatch)
END_INTERFACE_MAP()


// CDynamicGrammarDoc construction/destruction

CDynamicGrammarDoc::CDynamicGrammarDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CDynamicGrammarDoc::~CDynamicGrammarDoc()
{
	AfxOleUnlockApp();
}

BOOL CDynamicGrammarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	UpdateAllViews(NULL);
	return TRUE;
}




// CDynamicGrammarDoc serialization

void CDynamicGrammarDoc::Serialize(CArchive& ar)
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
void CDynamicGrammarDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CDynamicGrammarDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDynamicGrammarDoc::SetSearchContent(const CString& value)
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

// CDynamicGrammarDoc diagnostics

#ifdef _DEBUG
void CDynamicGrammarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynamicGrammarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDynamicGrammarDoc commands
