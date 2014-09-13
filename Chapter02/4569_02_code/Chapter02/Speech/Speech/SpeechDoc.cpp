
// SpeechDoc.cpp : implementation of the CSpeechDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Speech.h"
#endif

#include "SpeechDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSpeechDoc

IMPLEMENT_DYNCREATE(CSpeechDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpeechDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSpeechDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_ISpeech to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {EE3E3D41-A623-40D1-8BC9-C5EB88E720A6}
static const IID IID_ISpeech =
{ 0xEE3E3D41, 0xA623, 0x40D1, { 0x8B, 0xC9, 0xC5, 0xEB, 0x88, 0xE7, 0x20, 0xA6 } };

BEGIN_INTERFACE_MAP(CSpeechDoc, CDocument)
	INTERFACE_PART(CSpeechDoc, IID_ISpeech, Dispatch)
END_INTERFACE_MAP()


// CSpeechDoc construction/destruction

CSpeechDoc::CSpeechDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CSpeechDoc::~CSpeechDoc()
{
	AfxOleUnlockApp();
}

BOOL CSpeechDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	UpdateAllViews(NULL);
	return TRUE;
}




// CSpeechDoc serialization

void CSpeechDoc::Serialize(CArchive& ar)
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
void CSpeechDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CSpeechDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSpeechDoc::SetSearchContent(const CString& value)
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

// CSpeechDoc diagnostics

#ifdef _DEBUG
void CSpeechDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpeechDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpeechDoc commands
