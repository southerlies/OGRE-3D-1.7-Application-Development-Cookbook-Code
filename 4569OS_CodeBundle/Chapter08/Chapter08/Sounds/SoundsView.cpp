
// SoundsView.cpp : implementation of the CSoundsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Sounds.h"
#endif

#include "SoundsDoc.h"
#include "SoundsView.h"
#include "SoundController.h"

#include <sphelper.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSoundsView

IMPLEMENT_DYNCREATE(CSoundsView, CView)

BEGIN_MESSAGE_MAP(CSoundsView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CSoundsView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CSoundsView::OnAnimationStop)
END_MESSAGE_MAP()

// CSoundsView construction/destruction

CSoundsView::CSoundsView()
{
	m_First = true;
	m_MouseNavigation = false;
}

CSoundsView::~CSoundsView()
{
}

BOOL CSoundsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSoundsView drawing

void CSoundsView::OnDraw(CDC* /*pDC*/)
{
	CSoundsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CSoundsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSoundsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSoundsView diagnostics

#ifdef _DEBUG
void CSoundsView::AssertValid() const
{
	CView::AssertValid();
}

void CSoundsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSoundsDoc* CSoundsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSoundsDoc)));
	return (CSoundsDoc*)m_pDocument;
}
#endif //_DEBUG


// CSoundsView message handlers


void CSoundsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CSoundsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;


	CView::OnLButtonUp(nFlags, point);
}


void CSoundsView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CSoundsApp*)AfxGetApp())->m_Engine;
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


BOOL CSoundsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CSoundsApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSoundsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CSoundsDoc *Document = GetDocument();
	CEngine *Engine = ((CSoundsApp*)AfxGetApp())->m_Engine;
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
	}		// Do not call CView::OnPaint() for painting messages
}


void CSoundsView::OnTimer(UINT_PTR nIDEvent)
{
	m_WalkAnimation->addTime(0.001);

	CEngine *Engine = ((CSoundsApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}

void CSoundsView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSoundsApp*)AfxGetApp())->m_Engine->GetRoot();

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

	TCHAR szPath[MAX_PATH];

	CString ApplicationName = "Sounds.exe";

	GetModuleFileName(NULL, szPath, MAX_PATH);
    Ogre::ConfigFile OgreConfigFile;
	CString SoundPath(szPath);

	SoundPath = SoundPath.Left(SoundPath.GetLength() - ApplicationName.GetLength());
	SoundPath += L"\\..\\..\\media\\sounds";
	CSoundsApp* SoundsApp = (CSoundsApp*)AfxGetApp();
	CComPtr<ISpVoice> Voice = SoundsApp->m_cpVoice;

	ULONG ulCount = 0;
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

	m_WalkAnimation = RobotEntity->getAnimationState("Walk");
	m_WalkAnimation->setEnabled(true);

#undef new

	Ogre::ControllerFunctionRealPtr func(OGRE_NEW Ogre::WaveformControllerFunction(Ogre::WFT_SAWTOOTH));
	Ogre::ControllerValueRealPtr dest(OGRE_NEW CSoundController(SoundPath, Voice));

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	Ogre::ControllerManager& ControllerManager = Ogre::ControllerManager::getSingleton();
	m_SoundController = ControllerManager.createController(ControllerManager.getFrameTimeSource(), dest, func);

	Root->renderOneFrame();
}


void CSoundsView::OnAnimationStart()
{
	SetTimer(1,1,0);
}


void CSoundsView::OnAnimationStop()
{
	KillTimer(1);
}
