
// PoseAnimationView.cpp : implementation of the CPoseAnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PoseAnimation.h"
#endif

#include "PoseAnimationDoc.h"
#include "PoseAnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPoseAnimationView

IMPLEMENT_DYNCREATE(CPoseAnimationView, CView)

BEGIN_MESSAGE_MAP(CPoseAnimationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CPoseAnimationView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CPoseAnimationView::OnAnimationStop)
END_MESSAGE_MAP()

// CPoseAnimationView construction/destruction

CPoseAnimationView::CPoseAnimationView()
{
m_MouseNavigation = false;
}

CPoseAnimationView::~CPoseAnimationView()
{
}

BOOL CPoseAnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPoseAnimationView drawing

void CPoseAnimationView::OnDraw(CDC* /*pDC*/)
{
	CPoseAnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CPoseAnimationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPoseAnimationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPoseAnimationView diagnostics

void CPoseAnimationView::EngineSetup(void)
{
	Ogre::Root *Root = ((CPoseAnimationApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "PoseAnimation");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("PoseAnimation");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("PoseAnimation", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(200.0, 100.0, 500.0));
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
	m_Camera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
	
	Ogre::MeshPtr Mesh = Ogre::MeshManager::getSingleton().load("cube.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::SubMesh* SubMesh = Mesh->getSubMesh(0);
	Ogre::VertexDeclaration* VertexDeclaration = SubMesh->vertexData->vertexDeclaration->getAutoOrganisedDeclaration(true, true);
	SubMesh->vertexData->reorganiseBuffers(VertexDeclaration);

	Ogre::Pose* Pose = Mesh->createPose(1, "Pose1");

	Ogre::Vector3 offset1(0, 50, 0);

	Pose->addVertex(0, offset1);
	Pose->addVertex(1, offset1);
	Pose->addVertex(2, offset1);
	Pose->addVertex(3, offset1);

	Pose = Mesh->createPose(1, "pose2");
            // Pose2 moves vertices 3, 4, and 5 to the right
	Ogre::Vector3 offset2(100, 0, 0);

	Pose->addVertex(3, offset2);
	Pose->addVertex(4, offset2);
	Pose->addVertex(5, offset2);

// Creates a new Animation object for vertex animating this Mesh.
	Ogre::Animation* anim = Mesh->createAnimation( "PoseAnimation", 20 );
      
// Creates a VertexAnimationTrack for animating vertex position data. 
	Ogre::VertexAnimationTrack* track = anim->createVertexTrack( 1, Ogre::VAT_POSE );

// Creates the single pose KeyFrame and adds it to this animation.
	Ogre::VertexPoseKeyFrame *PoseAnimationKeyFrame = track->createVertexPoseKeyFrame( 0 );

            // Frame 1 - bring in pose 1 (index 0)
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(3);
    PoseAnimationKeyFrame->addPoseReference(0, 1.0f);

            // Frame 2 - remove all 
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(6);

            // Frame 3 - bring in pose 2 (index 1)
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(9);
    PoseAnimationKeyFrame->addPoseReference(1, 1.0f);

            // Frame 4 - remove all
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(12);
	
            // Frame 5 - bring in pose 1 at 50%, pose 2 at 100% 
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(15);
    PoseAnimationKeyFrame->addPoseReference(0, 0.5f);
    PoseAnimationKeyFrame->addPoseReference(1, 1.0f);

            // Frame 6 - bring in pose 1 at 100%, pose 2 at 50% 
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(18);
    PoseAnimationKeyFrame->addPoseReference(0, 1.0f);
    PoseAnimationKeyFrame->addPoseReference(1, 0.5f);

            // Frame 7 - reset
    PoseAnimationKeyFrame = track->createVertexPoseKeyFrame(20);

	Ogre::MeshSerializer Serializer;
    Serializer.exportMesh(Mesh.get(), "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\models\\testpose.mesh");

// Add a new pose reference.
	PoseAnimationKeyFrame->addPoseReference(0, 1.0f);
// create entity
	Ogre::Entity* PoseEntity = SceneManager->createEntity("Pose", "testpose.mesh");
// For entities based on animated meshes, gets the AnimationState object for a single animation.
	Ogre::AnimationState *PoseAnimationAnimState = PoseEntity->getAnimationState("PoseAnimation");

	m_AnimStateList.push_back(PoseAnimationAnimState);
 
// Sets the time position for this animation. 
	PoseAnimationAnimState->setTimePosition(0);
      
// Sets whether this animation is enabled.
	PoseAnimationAnimState->setEnabled( true );
	Ogre::SceneNode* PoseNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	PoseNode->attachObject(PoseEntity);
	m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);	

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CPoseAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CPoseAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPoseAnimationDoc* CPoseAnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPoseAnimationDoc)));
	return (CPoseAnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CPoseAnimationView message handlers


void CPoseAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CPoseAnimationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CPoseAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CPoseAnimationApp*)AfxGetApp())->m_Engine;
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


BOOL CPoseAnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CPoseAnimationApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CPoseAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CPoseAnimationDoc *Document = GetDocument();
	CEngine *Engine = ((CPoseAnimationApp*)AfxGetApp())->m_Engine;
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


void CPoseAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CPoseAnimationApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("PoseAnimation");
        
	std::vector<Ogre::AnimationState*>::iterator AnimIterator;
            
	for (AnimIterator = m_AnimStateList.begin(); AnimIterator != m_AnimStateList.end(); ++AnimIterator)
    {
       (*AnimIterator)->addTime(0.01);
    }

    Root->renderOneFrame();
	CView::OnTimer(nIDEvent);
}


void CPoseAnimationView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CPoseAnimationView::OnAnimationStop()
{
	KillTimer(1);
}
