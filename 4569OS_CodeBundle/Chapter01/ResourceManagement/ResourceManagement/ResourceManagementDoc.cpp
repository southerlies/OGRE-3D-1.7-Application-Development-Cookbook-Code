
// ResourceManagementDoc.cpp : implementation of the CResourceManagementDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ResourceManagement.h"
#endif

#include "ResourceManagementDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CResourceManagementDoc

IMPLEMENT_DYNCREATE(CResourceManagementDoc, CDocument)

BEGIN_MESSAGE_MAP(CResourceManagementDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CResourceManagementDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IResourceManagement to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {E8A91AB6-C9E7-4541-93B6-AFE1C95A1B56}
static const IID IID_IResourceManagement =
{ 0xE8A91AB6, 0xC9E7, 0x4541, { 0x93, 0xB6, 0xAF, 0xE1, 0xC9, 0x5A, 0x1B, 0x56 } };

BEGIN_INTERFACE_MAP(CResourceManagementDoc, CDocument)
	INTERFACE_PART(CResourceManagementDoc, IID_IResourceManagement, Dispatch)
END_INTERFACE_MAP()


// CResourceManagementDoc construction/destruction

CResourceManagementDoc::CResourceManagementDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CResourceManagementDoc::~CResourceManagementDoc()
{
	AfxOleUnlockApp();
}

BOOL CResourceManagementDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CResourceManagementDoc serialization

void CResourceManagementDoc::Serialize(CArchive& ar)
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
void CResourceManagementDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CResourceManagementDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CResourceManagementDoc::SetSearchContent(const CString& value)
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

// CResourceManagementDoc diagnostics

#ifdef _DEBUG
void CResourceManagementDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResourceManagementDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CResourceManagementDoc commands


BOOL CResourceManagementDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}
