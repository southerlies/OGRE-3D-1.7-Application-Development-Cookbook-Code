
// MirrorView.cpp : implementation of the CMirrorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Mirror.h"
#endif

#include "MirrorDoc.h"
#include "MirrorView.h"
#include "MirrorListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMirrorView

IMPLEMENT_DYNCREATE(CMirrorView, CView)

BEGIN_MESSAGE_MAP(CMirrorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CMirrorView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CMirrorView::OnAnimationStop)
END_MESSAGE_MAP()

// CMirrorView construction/destruction

CMirrorView::CMirrorView()
{
m_First = true;

m_MouseNavigation = false;
}

CMirrorView::~CMirrorView()
{
}

BOOL CMirrorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMirrorView drawing

void CMirrorView::OnDraw(CDC* /*pDC*/)
{
	CMirrorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CMirrorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMirrorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMirrorView diagnostics

#ifdef _DEBUG
void CMirrorView::AssertValid() const
{
	CView::AssertValid();
}

void CMirrorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMirrorDoc* CMirrorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMirrorDoc)));
	return (CMirrorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMirrorView message handlers


void CMirrorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CMirrorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CMirrorView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CMirrorApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	Ogre::Vector3 OldPosition = m_Camera->getPosition();
	m_Camera->moveRelative(CameraMove);
		
	m_MousePosition = point;

	Root->renderOneFrame();
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CMirrorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CMirrorApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMirrorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CMirrorDoc *Document = GetDocument();
	CEngine *Engine = ((CMirrorApp*)AfxGetApp())->m_Engine;
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
// Do not call CView::OnPaint() for painting messages
}


void CMirrorView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CMirrorApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CMirrorView::EngineSetup(void)
{
	Ogre::Root *Root = ((CMirrorApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MFCOgre");

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

	try
	{
		m_RenderWindow = Root->createRenderWindow("Ogre in MFC", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
	}
// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the camera
    m_Camera = m_SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(0.5);
	m_Camera->setFarClipDistance(5000); 
	m_Camera->setCastShadows(false);
	m_Camera->setUseRenderingDistance(true);
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::Entity *RobotEntity = m_SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	m_MirrorCamera = m_SceneManager->createCamera("MirrorCamera");
    m_MirrorCamera->setNearClipDistance(m_Camera->getNearClipDistance());
    m_MirrorCamera->setFarClipDistance(m_Camera->getFarClipDistance());
    m_MirrorCamera->setAspectRatio( (Ogre::Real)m_RenderWindow->getViewport(0)->getActualWidth() /
                         (Ogre::Real)m_RenderWindow->getViewport(0)->getActualHeight());

	m_MirrorCamera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));
	m_MirrorCamera->lookAt(Center);
	
	m_WalkAnimation = RobotEntity->getAnimationState("Walk");
	m_WalkAnimation->setEnabled(true);

	createMirror("Mirror","Mirror",m_MirrorCamera, 500, 500, Ogre::Vector3(100.0, 0.0, 0.0));
	Root->renderOneFrame();
}

void CMirrorView::createMirror(Ogre::String planeName,
                        Ogre::String texName,
                        Ogre::Camera* MirrorCamera,
                        Ogre::Real sizeX,
                        Ogre::Real sizeY,
                        Ogre::Vector3 position)
{
#undef new
    m_MirrorPlane = new Ogre::MovablePlane(planeName + "_mirrorplane");
#define new DEBUG_NEW
    m_MirrorPlane->d = 0;
    m_MirrorPlane->normal = Ogre::Vector3::UNIT_Y;

    Ogre::MeshManager::getSingleton().createPlane(planeName + "_mirrorplane",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      *m_MirrorPlane, sizeX, sizeY,
      1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

   m_MirrorEntity = m_SceneManager->createEntity(planeName + "_plane", planeName + "_mirrorplane" );

   Ogre::SceneNode *mMainNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();

   mMainNode->attachObject(m_MirrorEntity);
   mMainNode->attachObject(m_MirrorPlane);

   mMainNode->setPosition(position);
 
   Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(texName + "_rttrex",
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
         512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET );

   Ogre::RenderTarget *rttTex = texture->getBuffer()->getRenderTarget();
   
      Ogre::Viewport *v = rttTex->addViewport(MirrorCamera);
      v->setClearEveryFrame(true);
      v->setBackgroundColour(Ogre::ColourValue::White);

      Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(texName + "rttmat",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

//      Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("RustedMetal.jpg");
      Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState(texName + "_rttrex");

//	  t->setColourOperationEx(Ogre::LBX_BLEND_MANUAL, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
//		  Ogre::ColourValue::White,Ogre::ColourValue::White,0.25); 
      t->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
      t->setProjectiveTexturing(true, MirrorCamera);
	  rttTex->setAutoUpdated(true);

	  m_MirrorEntity->setMaterialName(mat.get()->getName());
	  MirrorListener *Listener = new MirrorListener;
	  Listener->m_PlaneEntity = m_MirrorEntity;
      rttTex->addListener(Listener);

      // set up linked reflection
      MirrorCamera->enableReflection(m_MirrorPlane);
      MirrorCamera->enableCustomNearClipPlane(m_MirrorPlane);
}

void CMirrorView::OnAnimationStart()
{
		SetTimer(1, 1, 0);
}


void CMirrorView::OnAnimationStop()
{
	KillTimer(1);
}
