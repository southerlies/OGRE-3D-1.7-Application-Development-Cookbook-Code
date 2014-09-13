
// TransparentMaterialView.cpp : implementation of the CTransparentMaterialView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TransparentMaterial.h"
#endif

#include "TransparentMaterialDoc.h"
#include "TransparentMaterialView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransparentMaterialView

IMPLEMENT_DYNCREATE(CTransparentMaterialView, CView)

BEGIN_MESSAGE_MAP(CTransparentMaterialView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTransparentMaterialView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CTransparentMaterialView construction/destruction

CTransparentMaterialView::CTransparentMaterialView()
{
	// TODO: add construction code here

}

CTransparentMaterialView::~CTransparentMaterialView()
{
}

BOOL CTransparentMaterialView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTransparentMaterialView drawing

void CTransparentMaterialView::OnDraw(CDC* /*pDC*/)
{
	CTransparentMaterialDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTransparentMaterialView printing


void CTransparentMaterialView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTransparentMaterialView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransparentMaterialView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransparentMaterialView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTransparentMaterialView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTransparentMaterialView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTransparentMaterialView diagnostics

#ifdef _DEBUG
void CTransparentMaterialView::AssertValid() const
{
	CView::AssertValid();
}

void CTransparentMaterialView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransparentMaterialDoc* CTransparentMaterialView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransparentMaterialDoc)));
	return (CTransparentMaterialDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransparentMaterialView message handlers
void CTransparentMaterialView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CTransparentMaterialDoc *Document = GetDocument();
	CEngine *Engine = ((CTransparentMaterialApp*)AfxGetApp())->m_Engine;
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


void CTransparentMaterialView::EngineSetup(void)
{
	Ogre::Root *Root = ((CTransparentMaterialApp*)AfxGetApp())->m_Engine->GetRoot();

	Ogre::SceneManager *SceneManager = NULL;

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MFCOgre");

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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Ogre in MFC");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Ogre in MFC", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(320.0, 240.0, 500.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	Ogre::ManualObject *Screen = SceneManager->createManualObject("Screen");
	Screen->setDynamic(true);
	Screen->begin("window", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Screen->position(-100,-100,50);
	Screen->textureCoord(0,0);

	Screen->position(300,-100,50);
	Screen->textureCoord(1,0);

	Screen->position(300,300,50);
	Screen->textureCoord(1,1);

	Screen->triangle(0, 1, 2);
		
	Screen->position(-100,-100,50);
	Screen->textureCoord(0,0);

	Screen->position(300,300,50);
	Screen->textureCoord(1,1);

	Screen->position(-100,300,50);
	Screen->textureCoord(0,1);

	Screen->triangle(3, 4, 5);
	 
	Screen->end();
	
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::SceneNode *WindowNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	WindowNode->attachObject(Screen);
}


BOOL CTransparentMaterialView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CTransparentMaterialApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
