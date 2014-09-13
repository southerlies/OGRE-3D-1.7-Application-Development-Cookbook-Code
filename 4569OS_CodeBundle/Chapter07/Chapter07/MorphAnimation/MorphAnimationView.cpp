
// MorphAnimationView.cpp : implementation of the CMorphAnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MorphAnimation.h"
#endif

#include "MorphAnimationDoc.h"
#include "MorphAnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMorphAnimationView

IMPLEMENT_DYNCREATE(CMorphAnimationView, CView)

BEGIN_MESSAGE_MAP(CMorphAnimationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CMorphAnimationView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CMorphAnimationView::OnAnimationStop)
END_MESSAGE_MAP()

// CMorphAnimationView construction/destruction

CMorphAnimationView::CMorphAnimationView()
{
m_MouseNavigation = false;
}

CMorphAnimationView::~CMorphAnimationView()
{
}

BOOL CMorphAnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMorphAnimationView drawing

void CMorphAnimationView::OnDraw(CDC* /*pDC*/)
{
	CMorphAnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CMorphAnimationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMorphAnimationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMorphAnimationView diagnostics

void CMorphAnimationView::EngineSetup(void)
{
	Ogre::Root *Root = ((CMorphAnimationApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MorphAnimation");
 
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
	m_Camera->setPosition(Ogre::Vector3(200.0, 200.0, 500.0));
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

	Ogre::VertexDeclaration* newVertexDeclaration = SubMesh->vertexData->vertexDeclaration->getAutoOrganisedDeclaration(false, true);
	SubMesh->vertexData->reorganiseBuffers(newVertexDeclaration);

	const Ogre::VertexElement* PositionElement = SubMesh->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
	Ogre::HardwareVertexBufferSharedPtr PositionBuffer = SubMesh->vertexData->vertexBufferBinding->getBuffer(PositionElement->getSource());

	Ogre::HardwareVertexBufferSharedPtr MorphBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer
	(
		Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3),
		SubMesh->vertexData->vertexCount, 
		Ogre::HardwareBuffer::HBU_STATIC, true
	);
	
	float* pSrc = static_cast<float*>(PositionBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	float* pDst = static_cast<float*>(MorphBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	for (size_t VertexIndex = 0; VertexIndex < SubMesh->vertexData->vertexCount; ++VertexIndex)
	{
		*pDst++ = (*pSrc++) + VertexIndex * 10.0f;
		*pDst++ = (*pSrc++) + 100.0f;
		*pDst++ = (*pSrc++) + VertexIndex * 10.0f;
	}

	PositionBuffer->unlock();
	MorphBuffer->unlock();
		
	Ogre::Animation* Animation = Mesh->createAnimation("testAnim", 10.0f);
	Ogre::VertexAnimationTrack* VertexTrack = Animation->createVertexTrack(1, SubMesh->vertexData, Ogre::VAT_MORPH);
	Ogre::VertexMorphKeyFrame* KeyFrame = VertexTrack->createVertexMorphKeyFrame(0);
	KeyFrame->setVertexBuffer(PositionBuffer);

	KeyFrame = VertexTrack->createVertexMorphKeyFrame(5.0f);
	KeyFrame->setVertexBuffer(MorphBuffer);

	KeyFrame = VertexTrack->createVertexMorphKeyFrame(10.0f);
	KeyFrame->setVertexBuffer(PositionBuffer);

	Ogre::MeshSerializer Serializer;
	Serializer.exportMesh(Mesh.get(), "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\models\\testmorph.mesh");

	Ogre::Entity* Entity = SceneManager->createEntity("test", "testmorph.mesh");
	SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(Entity);
	Ogre::AnimationState* animState = Entity->getAnimationState("testAnim");
	animState->setEnabled(true);
	animState->setWeight(1.0f);
	m_AnimStateList.push_back(animState);

	m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);	

	Root->renderOneFrame();
}

#ifdef _DEBUG
void CMorphAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CMorphAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMorphAnimationDoc* CMorphAnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMorphAnimationDoc)));
	return (CMorphAnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CMorphAnimationView message handlers


void CMorphAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CMorphAnimationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CMorphAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CMorphAnimationApp*)AfxGetApp())->m_Engine;
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


BOOL CMorphAnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CMorphAnimationApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMorphAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CMorphAnimationDoc *Document = GetDocument();
	CEngine *Engine = ((CMorphAnimationApp*)AfxGetApp())->m_Engine;
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


void CMorphAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CMorphAnimationApp*)AfxGetApp())->m_Engine;
	Ogre::Root *Root = Engine->GetRoot();
	Ogre::SceneManager *SceneManager = Root->getSceneManager("MorphAnimation");
        
	std::vector<Ogre::AnimationState*>::iterator AnimIterator;
            
	for (AnimIterator = m_AnimStateList.begin(); AnimIterator != m_AnimStateList.end(); ++AnimIterator)
    {
       (*AnimIterator)->addTime(0.01);
    }

    Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


void CMorphAnimationView::OnAnimationStart()
{
	SetTimer(1, 1, 0);
}


void CMorphAnimationView::OnAnimationStop()
{
	KillTimer(1);
}
