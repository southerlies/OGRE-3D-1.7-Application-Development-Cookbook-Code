
// SceletalAnimationView.cpp : implementation of the CSceletalAnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SceletalAnimation.h"
#endif

#include "SceletalAnimationDoc.h"
#include "SceletalAnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSceletalAnimationView

IMPLEMENT_DYNCREATE(CSceletalAnimationView, CView)

BEGIN_MESSAGE_MAP(CSceletalAnimationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CSceletalAnimationView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CSceletalAnimationView::OnAnimationStop)
END_MESSAGE_MAP()

// CSceletalAnimationView construction/destruction

CSceletalAnimationView::CSceletalAnimationView()
{
	m_MouseNavigation = false;
	m_TimePosition = 0.0;
//	m_TimeIndex = 0;
}

CSceletalAnimationView::~CSceletalAnimationView()
{
}

BOOL CSceletalAnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSceletalAnimationView drawing

void CSceletalAnimationView::OnDraw(CDC* /*pDC*/)
{
	CSceletalAnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CSceletalAnimationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSceletalAnimationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSceletalAnimationView diagnostics

void CSceletalAnimationView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSceletalAnimationApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Animation");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Mouse Input");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Mouse Input", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(5.0, 5.0, 10.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
	m_Camera->lookAt(Ogre::Vector3(0.5, 0.5, 0.5));
	m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);

	Ogre::ManualObject* ManualObject = NULL;
	ManualObject = SceneManager->createManualObject("Animation");
	ManualObject->setDynamic(false);
    ManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//face 1
	ManualObject->position(0, 0, 0);//0
	ManualObject->position(1, 0, 0);//1
	ManualObject->position(1, 1, 0);//2
	ManualObject->triangle(0, 1, 2);//3
	
	ManualObject->position(0, 0, 0);//4
	ManualObject->position(1, 1, 0);//5
	ManualObject->position(0, 1, 0);//6
	ManualObject->triangle(3, 4, 5);//7
	//face 2
	ManualObject->position(0, 0, 1);//8
	ManualObject->position(1, 0, 1);//9
	ManualObject->position(1, 1, 1);//10
	ManualObject->triangle(6, 7, 8);//11

	ManualObject->position(0, 0, 1);//12
	ManualObject->position(1, 1, 1);//13
	ManualObject->position(0, 1, 1);//14
	ManualObject->triangle(9, 10, 11);//15
	//face 3
	ManualObject->position(0, 0, 0);//16
	ManualObject->position(1, 0, 0);//17
	ManualObject->position(1, 0, 1);//18
	ManualObject->triangle(12, 13, 14);//19

	ManualObject->position(0, 0, 0);
	ManualObject->position(1, 0, 1);
	ManualObject->position(0, 1, 1);
	ManualObject->triangle(15, 16, 17);
	//face 4
	ManualObject->position(1, 0, 0);
	ManualObject->position(1, 1, 0);
	ManualObject->position(1, 1, 1);
	ManualObject->triangle(18, 19, 20);

	ManualObject->position(1, 0, 0);
	ManualObject->position(1, 1, 1);
	ManualObject->position(1, 0, 1);
	ManualObject->triangle(21, 22, 23);
	//face 5
	ManualObject->position(0, 1, 0);
	ManualObject->position(1, 1, 0);
	ManualObject->position(0, 1, 1);
	ManualObject->triangle(24, 25, 26);

	ManualObject->position(1, 1, 0);
	ManualObject->position(1, 1, 1);
	ManualObject->position(0, 1, 1);
	ManualObject->triangle(27, 28, 29);
	
	//face 6
	ManualObject->position(0, 0, 0);
	ManualObject->position(0, 1, 1);
	ManualObject->position(0, 0, 1);
	ManualObject->triangle(30, 31, 32);

	ManualObject->position(0, 0, 0);
	ManualObject->position(0, 1, 0);
	ManualObject->position(0, 1, 1);
	ManualObject->triangle(33, 34, 35);

	ManualObject->end();
	Ogre::MeshPtr MeshPtr = ManualObject->convertToMesh("Animation");
	Ogre::SubMesh* sub = MeshPtr->getSubMesh(0);
	
	Ogre::SkeletonPtr Skeleton = Ogre::SkeletonManager::getSingleton().create("Skeleton", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	MeshPtr.getPointer()->_notifySkeleton(Skeleton);
	Ogre::Bone *Root1 = NULL;
	Ogre::Bone *Child1 = NULL;
	Ogre::Bone *Child2 = NULL;

	Root1 = Skeleton.getPointer()->createBone("Root");
	Root1->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
	Root1->setOrientation(Ogre::Quaternion::IDENTITY);
	
	Child1 = Root1->createChild(1);
	Child1->setPosition(Ogre::Vector3(4.0, 0.0, 0.0));
	Child1->setOrientation(Ogre::Quaternion::IDENTITY);
	Child2 = Root1->createChild(2);
	Child2->setPosition(Ogre::Vector3(5.0, 0.0, 0.0));
	Child2->setOrientation(Ogre::Quaternion::IDENTITY);

	Ogre::VertexBoneAssignment Assignment;

	Assignment.boneIndex = 0;
	Assignment.vertexIndex = 0;
	Assignment.weight = 1.0;
	Skeleton->setBindingPose();

	sub->addBoneAssignment(Assignment);

	Assignment.vertexIndex = 1;
	sub->addBoneAssignment(Assignment);

	Assignment.vertexIndex = 2;
	sub->addBoneAssignment(Assignment);

	Ogre::Animation *Animation = MeshPtr->createAnimation("HandAnimation", 100.0);
	Ogre::NodeAnimationTrack *Track = Animation->createNodeTrack(0, Root1);
	Ogre::TransformKeyFrame *KeyFrame = NULL;

	for (float FrameTime = 0.0; FrameTime < 100.0; FrameTime += 0.1)
	{
		KeyFrame = Track->createNodeKeyFrame(FrameTime);
		KeyFrame->setTranslate(Ogre::Vector3(10.0, 0.0, 0.0));
	}

	Root1->setManuallyControlled(true);
	Child1->setManuallyControlled(true);
	Child2->setManuallyControlled(true);
	MeshPtr->load();

	MeshPtr.getPointer()->_notifySkeleton(Skeleton);
		
//	Ogre::SkeletonSerializer skeletonSerializer;
//	skeletonSerializer.exportSkeleton(Skeleton.get(), "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\models\\testskeleton.skeleton");
//	Ogre::MeshSerializer ser;
//    ser.exportMesh(MeshPtr.get(), "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\models\\testskeleton.mesh");

	Ogre::Entity *Entity = SceneManager->createEntity("Animation", "Animation"/*"testskeleton.mesh"*/);
	Ogre::SceneNode *SceneNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	SceneNode->attachObject(Entity);
	Entity->setDisplaySkeleton(true);

	m_AnimationState = Entity->getAnimationState("HandAnimation");
	m_AnimationState->setEnabled(true);
	m_AnimationState->setLoop(true);
	
	m_Camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
	 
	Root->renderOneFrame();
}

#ifdef _DEBUG
void CSceletalAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CSceletalAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSceletalAnimationDoc* CSceletalAnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSceletalAnimationDoc)));
	return (CSceletalAnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CSceletalAnimationView message handlers


void CSceletalAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CSceletalAnimationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CSceletalAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{

	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CSceletalAnimationApp*)AfxGetApp())->m_Engine;
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


BOOL CSceletalAnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CSceletalAnimationApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSceletalAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CSceletalAnimationDoc *Document = GetDocument();
	CEngine *Engine = ((CSceletalAnimationApp*)AfxGetApp())->m_Engine;
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


void CSceletalAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CSceletalAnimationApp*)AfxGetApp())->m_Engine;		
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("Animation");
	Ogre::Entity *Entity = SceneManager->getEntity("Animation");
	Ogre::Skeleton *Skeleton = Entity->getSkeleton();

	m_AnimationState->addTime(0.1);
	Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CSceletalAnimationView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CSceletalAnimationView::OnAnimationStop()
{
	KillTimer(1);
}
