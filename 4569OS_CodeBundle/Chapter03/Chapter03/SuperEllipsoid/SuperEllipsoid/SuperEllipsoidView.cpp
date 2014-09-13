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

// SuperEllipsoidView.cpp : implementation of the CSuperEllipsoidView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SuperEllipsoid.h"
#endif

#include "SuperEllipsoidDoc.h"
#include "SuperEllipsoidView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSuperEllipsoidView

IMPLEMENT_DYNCREATE(CSuperEllipsoidView, CView)

BEGIN_MESSAGE_MAP(CSuperEllipsoidView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_HORIZONTAL, &CSuperEllipsoidView::OnHorizontal)
	ON_COMMAND(ID_VERTICAL, &CSuperEllipsoidView::OnVertical)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SYSKEYDOWN()
	ON_COMMAND(ID_HORIZONTAL_EDIT, &CSuperEllipsoidView::OnHorizontalEdit)
	ON_COMMAND(ID_VERTICAL_EDIT, &CSuperEllipsoidView::OnVerticalEdit)
END_MESSAGE_MAP()

// CSuperEllipsoidView construction/destruction

CSuperEllipsoidView::CSuperEllipsoidView()
{
	m_MouseNavigation = false;
}

CSuperEllipsoidView::~CSuperEllipsoidView()
{
}

BOOL CSuperEllipsoidView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSuperEllipsoidView drawing

void CSuperEllipsoidView::OnDraw(CDC* /*pDC*/)
{
	CSuperEllipsoidDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CSuperEllipsoidView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSuperEllipsoidView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSuperEllipsoidView diagnostics
// CMouseInputView diagnostics

void CSuperEllipsoidView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "SuperEllipsoid");
 
    //
    // Create a render window
    // This window should be the current ChildView window using the externalWindowHandle
    // value pair option.
    //

    Ogre::NameValuePairList parms;
    parms["externalWindowHandle"] = Ogre::StringConverter::toString((long)m_hWnd);
    parms["vsync"] = "true";

	CRect   rect;
    GetClientRect(&rect);
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("SuperEllipsoid");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("SuperEllipsoid", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	m_SuperQuadric.DrawMesh();
}

#ifdef _DEBUG
void CSuperEllipsoidView::AssertValid() const
{
	CView::AssertValid();
}

void CSuperEllipsoidView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSuperEllipsoidDoc* CSuperEllipsoidView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSuperEllipsoidDoc)));
	return (CSuperEllipsoidDoc*)m_pDocument;
}
#endif //_DEBUG


// CSuperEllipsoidView message handlers


void CSuperEllipsoidView::OnHorizontal()
{
	CMainFrame *MainFrame = (CMainFrame *)((CSuperEllipsoidApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* Slider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_HORIZONTAL));  
	CMFCRibbonEdit* Edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, RibbonBar->FindByID(ID_HORIZONTAL_EDIT));

	m_SuperQuadric.m_HorFactor = (double)Slider->GetPos() / 10.0;
	CString Text;
	Text.Format("%.2f", m_SuperQuadric.m_HorFactor);
	Edit->SetEditText(Text);
	m_SuperQuadric.UpdateMesh();

	CEngine *Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CSuperEllipsoidView::OnVertical()
{
	CMainFrame *MainFrame = (CMainFrame *)((CSuperEllipsoidApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* Slider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_VERTICAL));  
	CMFCRibbonEdit* Edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, RibbonBar->FindByID(ID_VERTICAL_EDIT));

	m_SuperQuadric.m_VertFactor = (double)Slider->GetPos() / 10.0;
	CString Text;
	Text.Format("%.2f", m_SuperQuadric.m_VertFactor);
	Edit->SetEditText(Text);
	m_SuperQuadric.UpdateMesh();

	CEngine *Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CSuperEllipsoidView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CSuperEllipsoidDoc *Document = GetDocument();
	CEngine *Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
	  return;

	Ogre::Root *Root = Engine->GetRoot();

	if (m_First && Root != NULL)
    {
        m_First = false;
        EngineSetup();
    }

	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CSuperEllipsoidView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -0.01 * (m_MousePosition.x - point.x);
	CameraMove[1] = 0.01 * m_MousePosition.y - point.y;

	CEngine * Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	m_Camera->moveRelative(CameraMove);

	m_MousePosition = point;

	Root->renderOneFrame();
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CSuperEllipsoidView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.05 * zDelta;

	CEngine * Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSuperEllipsoidView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CSuperEllipsoidView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CSuperEllipsoidView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'W':

		m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);

		break;

	case 'S':

		m_Camera->setPolygonMode(Ogre::PM_SOLID);

		break;

	case 'P':

		m_Camera->setPolygonMode(Ogre::PM_POINTS);

		break;
	}

	Ogre::Root *Root = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void CSuperEllipsoidView::OnHorizontalEdit()
{
	CMainFrame *MainFrame = (CMainFrame *)((CSuperEllipsoidApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* Slider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_HORIZONTAL));  
	CMFCRibbonEdit* Edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, RibbonBar->FindByID(ID_HORIZONTAL_EDIT));

	CString Text;
	Text = Edit->GetEditText();

	m_SuperQuadric.m_HorFactor = atof(Text);
	Slider->SetPos(m_SuperQuadric.m_HorFactor * 10.0);
	m_SuperQuadric.UpdateMesh();

	CEngine *Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}


void CSuperEllipsoidView::OnVerticalEdit()
{
	CMainFrame *MainFrame = (CMainFrame *)((CSuperEllipsoidApp*)AfxGetApp())->GetMainWnd();
	CMFCRibbonBar* RibbonBar = MainFrame->GetRibbonBar();

	CMFCRibbonSlider* Slider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, RibbonBar->FindByID(ID_VERTICAL));  
	CMFCRibbonEdit* Edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, RibbonBar->FindByID(ID_VERTICAL_EDIT));
	CString Text;
	Text = Edit->GetEditText();

	m_SuperQuadric.m_VertFactor = atof(Text);
	Slider->SetPos(m_SuperQuadric.m_VertFactor * 10.0);
	m_SuperQuadric.UpdateMesh();

	CEngine *Engine = ((CSuperEllipsoidApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	if (Root != NULL)
    {
       Root->renderOneFrame();
	}
}
