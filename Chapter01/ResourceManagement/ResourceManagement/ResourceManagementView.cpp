
// ResourceManagementView.cpp : implementation of the CResourceManagementView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ResourceManagement.h"
#endif

#include "ResourceManagementDoc.h"
#include "ResourceManagementView.h"
#include "StlFileManager.h"
#include "Ogre.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResourceManagementView

IMPLEMENT_DYNCREATE(CResourceManagementView, CView)

BEGIN_MESSAGE_MAP(CResourceManagementView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CResourceManagementView construction/destruction

CResourceManagementView::CResourceManagementView()
{
	// TODO: add construction code here

}

CResourceManagementView::~CResourceManagementView()
{
}

BOOL CResourceManagementView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CResourceManagementView drawing

void CResourceManagementView::OnDraw(CDC* /*pDC*/)
{
	CResourceManagementDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CResourceManagementView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CResourceManagementView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CResourceManagementView diagnostics

#ifdef _DEBUG
void CResourceManagementView::AssertValid() const
{
	CView::AssertValid();
}

void CResourceManagementView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CResourceManagementDoc* CResourceManagementView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResourceManagementDoc)));
	return (CResourceManagementDoc*)m_pDocument;
}
#endif //_DEBUG


// CResourceManagementView message handlers


void CResourceManagementView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CResourceManagementDoc *Document = GetDocument();
	CEngine *Engine = ((CResourceManagementApp*)AfxGetApp())->m_Engine;
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


void CResourceManagementView::EngineSetup(void)
{
	CEngine* Engine = ((CResourceManagementApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();

	Ogre::SceneManager *SceneManager = NULL;

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "ResourceManagement");

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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Resource Management");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Resource Management", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
	}
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	m_Camera->lookAt(0.0, 0.0, 0.0);
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
	Ogre::StringVector Groups = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();

	Ogre::Entity *MobiusEntity = Engine->m_StlFileManager->createEntity("Mobius", "1_4.stl");
	Ogre::SceneNode *MobiusNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	MobiusNode->attachObject(MobiusEntity);
	Ogre::AxisAlignedBox MobiusBox = MobiusEntity->getBoundingBox();
	Ogre::Vector3 MobiusCenter = MobiusBox.getCenter();
	m_Camera->lookAt(MobiusCenter);

	Ogre::Real Radius = MobiusEntity->getBoundingRadius();
	m_Camera->setPosition(300, 100, 200);
	m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);
}


