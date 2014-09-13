
// VoiceView.cpp : implementation of the CVoiceView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Voice.h"
#endif

#include "VoiceDoc.h"
#include "VoiceView.h"
#include <sphelper.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVoiceView

IMPLEMENT_DYNCREATE(CVoiceView, CView)

BEGIN_MESSAGE_MAP(CVoiceView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_COMMAND(ID_ANIMATION_START, &CVoiceView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CVoiceView::OnAnimationStop)
END_MESSAGE_MAP()

// CVoiceView construction/destruction

CVoiceView::CVoiceView()
{
m_First = true;

m_MouseNavigation = false;
}

CVoiceView::~CVoiceView()
{
}

BOOL CVoiceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVoiceView drawing

void CVoiceView::OnDraw(CDC* /*pDC*/)
{
	CVoiceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CVoiceView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVoiceView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVoiceView diagnostics

#ifdef _DEBUG
void CVoiceView::AssertValid() const
{
	CView::AssertValid();
}

void CVoiceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVoiceDoc* CVoiceView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVoiceDoc)));
	return (CVoiceDoc*)m_pDocument;
}
#endif //_DEBUG


// CVoiceView message handlers
void CVoiceView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CVoiceView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CVoiceView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CVoiceApp*)AfxGetApp())->m_Engine;
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


BOOL CVoiceView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CVoiceApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CVoiceView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1: //main loop

			switch(m_Event)
			{
				case 0:

					SetTimer(2,1,0);

					m_Voice->SetVoice(m_cpVoiceToken1);
					m_Voice->Speak(L"C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\bin\\debug\\Hamlet 0.txt", SPF_ASYNC | SPF_IS_FILENAME, NULL); 
					m_Event++;
					break;
				case 1:

//					KillTimer(2);
					SetTimer(3,1,0);

					m_Voice->SetVoice(m_cpVoiceToken2);
					m_Voice->Speak(L"C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\bin\\debug\\Hamlet 1.txt", SPF_ASYNC | SPF_IS_FILENAME, NULL); 

					m_Event++;
					break;
				case 2:

//					KillTimer(3);
					SetTimer(2,1,0);

					m_Voice->SetVoice(m_cpVoiceToken1);
					m_Voice->Speak(L"C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\bin\\debug\\Hamlet 2.txt", SPF_ASYNC | SPF_IS_FILENAME, NULL); 
					m_Event++;
					break;
				case 3:
//					KillTimer(2);
					SetTimer(3,1,0);

					m_Voice->SetVoice(m_cpVoiceToken2);
					m_Voice->Speak(L"C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\bin\\debug\\Hamlet 3.txt", SPF_ASYNC | SPF_IS_FILENAME, NULL); 
					m_Event++;
					break;
				default:
//					KillTimer(1);
//					KillTimer(2);
//					KillTimer(3);
					break;
			}
			break;

		case 2: //first robot speaking

			m_WalkAnimation1->addTime(0.01);

			break;

		case 3: //second robot speaking

			m_WalkAnimation2->addTime(0.01);

			break;
	}

	CEngine *Engine = ((CVoiceApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CVoiceView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CVoiceDoc *Document = GetDocument();
	CEngine *Engine = ((CVoiceApp*)AfxGetApp())->m_Engine;
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
void CVoiceView::EngineSetup(void)
{
	Ogre::Root *Root = ((CVoiceApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Voice");
		
	m_Voice = ((CVoiceApp*)AfxGetApp())->m_cpVoice;

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
		m_RenderWindow = Root->createRenderWindow("Voice", rect.Width(), rect.Height(), false, &parms);
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
		
	Ogre::Entity *RobotEntity1 = m_SceneManager->createEntity("Robot1", "robot.mesh");
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-100, 0, 0));
	RobotNode->attachObject(RobotEntity1);

	Ogre::Entity *RobotEntity2 = m_SceneManager->createEntity("Robot2", "robot.mesh");
	RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 0, 0));
	RobotNode->attachObject(RobotEntity2);

	m_Camera->lookAt(Ogre::Vector3(0, 0, 0));

	SpEnumTokens(SPCAT_VOICES, L"Name=Microsoft Anna", NULL, &m_cpEnum1);
	m_cpEnum1->Next(1, &m_cpVoiceToken1, NULL );

	SpEnumTokens(SPCAT_VOICES, L"Name=Microsoft Anna", NULL, &m_cpEnum2);
	m_cpEnum2->Next(1, &m_cpVoiceToken2, NULL );

	m_WalkAnimation1 = RobotEntity1->getAnimationState("Walk");
	m_WalkAnimation2 = RobotEntity2->getAnimationState("Walk");

	m_WalkAnimation1->setEnabled(true);
	m_WalkAnimation2->setEnabled(true);

	Root->renderOneFrame();
}



void CVoiceView::OnAnimationStart()
{
	m_Event = 0;
	SetTimer(1,1,0);
}


void CVoiceView::OnAnimationStop()
{
	// TODO: Add your command handler code here
}
