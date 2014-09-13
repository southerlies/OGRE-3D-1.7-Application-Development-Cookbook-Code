
// LightsView.cpp : implementation of the CLightsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lights.h"
#endif

#include "LightsDoc.h"
#include "LightsView.h"
#include "Cone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLightsView

IMPLEMENT_DYNCREATE(CLightsView, CView)

BEGIN_MESSAGE_MAP(CLightsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLightsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_LIGHTSCONTROL_LIGHTSCONTROL, &CLightsView::OnLightsControlLightsControl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_COMMAND(ID_LIGHTSCONTROL_1, &CLightsView::OnLightscontrol1)
	ON_COMMAND(ID_LIGHTSCONTROL_2, &CLightsView::OnLightscontrol2)
	ON_COMMAND(ID_LIGHTSCONTROL_3, &CLightsView::OnLightscontrol3)
	ON_COMMAND(ID_LIGHTSCONTROL_TURNON, &CLightsView::OnLightscontrolTurnon)
	ON_COMMAND(ID_LIGHTSCONTROL_TURNOFF, &CLightsView::OnLightscontrolTurnoff)
END_MESSAGE_MAP()

// CLightsView construction/destruction

CLightsView::CLightsView()
{
	m_First = true;
	m_LightsControlDlg = NULL;
}

CLightsView::~CLightsView()
{
}

BOOL CLightsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLightsView drawing

void CLightsView::OnDraw(CDC* /*pDC*/)
{
	CLightsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CLightsView printing


void CLightsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLightsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLightsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLightsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLightsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLightsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLightsView diagnostics

#ifdef _DEBUG
void CLightsView::AssertValid() const
{
	CView::AssertValid();
}

void CLightsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLightsDoc* CLightsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLightsDoc)));
	return (CLightsDoc*)m_pDocument;
}
#endif //_DEBUG


// CLightsView message handlers


void CLightsView::OnLightsControlLightsControl()
{
	if (m_LightsControlDlg == NULL)
	{
		m_LightsControlDlg = new CLightsControlDlg();
		m_LightsControlDlg->Create(IDD_LIGHTS_CONTROL);
	}

	m_LightsControlDlg->ShowWindow(SW_SHOW);
}


void CLightsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CLightsDoc *Document = GetDocument();
	CEngine *Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
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


void CLightsView::EngineSetup(void)
{
	Ogre::Root *Root = ((CLightsApp*)AfxGetApp())->m_Engine->GetRoot();

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Lights");

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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Lights");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Lights", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	SceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	SceneManager->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	SceneManager->setAmbientLight(Ogre::ColourValue(0.15, 0.15, 0.15));

    // Create the camera
    m_Camera = SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(0.0, 180.0, 400.0));
	//m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");
	m_Camera->lookAt(Ogre::Vector3(0,50.0,0));

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	// floor mesh
	Ogre::Plane Floor(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr WallMesh = Ogre::MeshManager::getSingleton().createPlane("Floor",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Floor,
            1000,1000,100,100,true,1,5,5, Ogre::Vector3::UNIT_Z);
	
	Ogre::Entity *FloorEntity = SceneManager->createEntity("Floor", "Floor");
	FloorEntity->setCastShadows(false);
	Ogre::SceneNode *FloorNode = SceneManager->getRootSceneNode()->createChildSceneNode("Floor");
	FloorNode->attachObject(FloorEntity);

	// spotlight can
	CCone ConeObject;
	ConeObject.m_Height = 20.0;
	ConeObject.m_Radius = 10.0;
	Ogre::ManualObject *Can = ConeObject.CreateCone(0,"SpotLightLight","Wall/Black");
	Ogre::SceneNode *CanNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(100.0, 181.0, 0.0));
	//Can->setCastShadows(true);
	CanNode->attachObject(Can);
	
	// spotlight beam
	ConeObject.m_Height = 200.0;
	ConeObject.m_Radius = 80.0;
	Ogre::ManualObject *Beam = ConeObject.CreateCone(0.99,"SpotLightBeam","LightBeam",0.5);
	Ogre::SceneNode *BeamNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(100.0, 0.0, 0.0));
	Beam->setCastShadows(false);
	BeamNode->attachObject(Beam);
	
	// spotlight light
	Ogre::Light* SpotLight = SceneManager->createLight("SpotLight");
	SpotLight->setDirection((Ogre::Vector3(0.0, 0.0, 0.0) - Ogre::Vector3(0.0, 100.0, 0.0)).normalisedCopy());
	SpotLight->setType(Ogre::Light::LT_SPOTLIGHT);          
	SpotLight->setDiffuseColour(1.0, 1.0, 0.0);      
	SpotLight->setSpecularColour(1.0, 1.0, 0.0);
	//SpotLight->setAttenuation(150, 1.0, 0.005, 0.0);
	SpotLight->setSpotlightRange(Ogre::Radian(0.5),Ogre::Radian(0.9),2.0f);
	SpotLight->setVisible(true);
	Ogre::SceneNode *LightNode = SceneManager->getRootSceneNode()->createChildSceneNode("SpotLight");

	LightNode->attachObject(SpotLight);
	LightNode->setPosition(Ogre::Vector3(100.0,199,0.0));

	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(100.0,00.0,0.0));
	RobotNode->yaw(-Ogre::Radian(Ogre::Math::HALF_PI));
	RobotEntity->setCastShadows(true);
	RobotNode->attachObject(RobotEntity);

	// point light
	Ogre::Light* PointLight = SceneManager->createLight("PointLight");
	PointLight->setType(Ogre::Light::LT_POINT);          
	PointLight->setDiffuseColour(1.0, 0.0, 0.0);      
	PointLight->setSpecularColour(1.0, 0.0, 0.0);    
	PointLight->setVisible(true);
	PointLight->setAttenuation(3250.0,1.0,0.0014,0.000007);
	Ogre::SceneNode *PointLightNode = SceneManager->getRootSceneNode()->createChildSceneNode("PointLight");

	PointLightNode->attachObject(PointLight);
	PointLightNode->setPosition(Ogre::Vector3(-100.0,150,30.0));

	// attach a flare to the point light node
	Ogre::BillboardSet* FlareSet = SceneManager->createBillboardSet("FlareSet");
	FlareSet->setMaterialName("Examples/FlarePointSprite");
	FlareSet->setDefaultDimensions(50.0,50.0);
	Ogre::Billboard* Flare = FlareSet->createBillboard(Ogre::Vector3(0,0,0),Ogre::ColourValue(1.0,0.0,0.0,0.5));
	PointLightNode->attachObject(FlareSet);

	Ogre::Entity *RobotEntity2 = SceneManager->createEntity("Robot2", "robot.mesh");
	Ogre::SceneNode *RobotNode2 = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-100.0,00.0,0.0));
	RobotNode2->yaw(-Ogre::Radian(Ogre::Math::HALF_PI));
	RobotEntity2->setCastShadows(true);
	RobotNode2->attachObject(RobotEntity2);

}


void CLightsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


void CLightsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}


void CLightsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
}


BOOL CLightsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CLightsView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnTimer(nIDEvent);
}


void CLightsView::OnLightscontrol1()
{
	Ogre::Entity *WallEntity = SceneManager->getEntity("Wall");
	WallEntity->setMaterialName("Wall/White");
	Ogre::SceneNode *WallNode = SceneManager->getSceneNode("Wall");
	WallNode->setVisible(true);
	CEngine * Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Root->renderOneFrame();
}


void CLightsView::OnLightscontrol2()
{
	Ogre::Entity *WallEntity = SceneManager->getEntity("Wall");
	WallEntity->setMaterialName("Wall/Grey");
	Ogre::SceneNode *WallNode = SceneManager->getSceneNode("Wall");
	WallNode->setVisible(true);

	CEngine * Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Root->renderOneFrame();
}

void CLightsView::OnLightscontrol3()
{
	Ogre::Entity *WallEntity = SceneManager->getEntity("Wall");
	WallEntity->setMaterialName("Wall/Black");
	Ogre::SceneNode *WallNode = SceneManager->getSceneNode("Wall");
	WallNode->setVisible(true);
	CEngine * Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Root->renderOneFrame();
}


void CLightsView::OnLightscontrolTurnon()
{
	Ogre::SceneNode *ScreenNode = SceneManager->getSceneNode("Screen");
	ScreenNode->setVisible(true);
	CEngine * Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Root->renderOneFrame();
}


void CLightsView::OnLightscontrolTurnoff()
{
	Ogre::SceneNode *WallNode = SceneManager->getSceneNode("Wall");
	WallNode->setVisible(false);
	Ogre::SceneNode *ScreenNode = SceneManager->getSceneNode("Screen");
	ScreenNode->setVisible(false);
	CEngine * Engine = ((CLightsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Root->renderOneFrame();
}
