
// AddingTreesView.cpp : implementation of the CAddingTreesView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AddingTrees.h"
#endif

#include "AddingTreesDoc.h"
#include "AddingTreesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAddingTreesView

IMPLEMENT_DYNCREATE(CAddingTreesView, CView)

BEGIN_MESSAGE_MAP(CAddingTreesView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CAddingTreesView construction/destruction

CAddingTreesView::CAddingTreesView()
{
	m_Camera = NULL;
	m_MouseNavigation = false;
}

CAddingTreesView::~CAddingTreesView()
{
}

BOOL CAddingTreesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAddingTreesView drawing

void CAddingTreesView::OnDraw(CDC* /*pDC*/)
{
	CAddingTreesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CAddingTreesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAddingTreesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAddingTreesView diagnostics

#ifdef _DEBUG
void CAddingTreesView::AssertValid() const
{
	CView::AssertValid();
}

void CAddingTreesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAddingTreesDoc* CAddingTreesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAddingTreesDoc)));
	return (CAddingTreesDoc*)m_pDocument;
}
#endif //_DEBUG


// CAddingTreesView message handlers


void CAddingTreesView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CAddingTreesDoc *Document = GetDocument();
	CEngine *Engine = ((CAddingTreesApp*)AfxGetApp())->m_Engine;
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

void CAddingTreesView::EngineSetup(void)
{
	Ogre::FloatRect TextureCoordinates[]=
	{
		Ogre::FloatRect(113.0/5000.0,121.0/5000.0,851.0/5000.0,1073.0/5000.0),
		Ogre::FloatRect(1021.0/5000.0,114.0/5000.0,3386.0/5000.0,1984.0/5000.0),
		Ogre::FloatRect(3825.0/5000.0,1049.0/5000.0,4871.0/5000.0,3588.0/5000.0),
		Ogre::FloatRect(1739.0/5000.0,2418.0/5000.0,2796.0/5000.0,4774.0/5000.0),
		Ogre::FloatRect(221.0/5000.0,2723.0/5000.0,1464.0/5000.0,3795.0/5000.0),
		Ogre::FloatRect(505.0/5000.0,4391.0/5000.0,805.0/5000.0,4662.0/5000.0),
		Ogre::FloatRect(339.0/5000.0,2085.0/5000.0,482.0/5000.0,2216.0/5000.0),
		Ogre::FloatRect(2803.0/5000.0,3355.0/5000.0,3891.0/5000.0,4912.0/5000.0)
	};

	Ogre::Root *Root = ((CAddingTreesApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "AddingTrees");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("AddingTrees");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("AddingTrees", rect.Width(), rect.Height(), false, &parms);
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
		Viewport->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
/**/
	Ogre::BillboardSet *Trees = SceneManager->createBillboardSet("Trees");
		
	Trees->setTextureCoords(TextureCoordinates, 8);
	Trees->setMaterialName("Trees");
	Trees->setCastShadows(true);
	Trees->setSortingEnabled(true);
	Trees->setBillboardType(Ogre::BBT_ORIENTED_COMMON);

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	double TreeWidth;
	double TreeHeight;
	int TextureIndex;

	for (int i = 0; i < 40; i++)
		for (int j = 0; j < 40; j++)
	{
		x = i * 5;
		y = j * 5;
		z = 0;
		
		TextureIndex = (i + j) / 10;
		TreeWidth = (i + j + 10) / 10;
		TreeHeight = (i + j + 10) / 5;
		
		Ogre::Vector3 TreePosition(x, y, z);
		Ogre::Billboard* Tree = Trees->createBillboard(TreePosition);
		Tree->setDimensions(TreeWidth, TreeHeight);
		Tree->setTexcoordIndex(TextureIndex);
	}

	Ogre::SceneNode *TreeNode = SceneManager->getRootSceneNode()->createChildSceneNode();

	TreeNode->setPosition(0, 0, 0);
	TreeNode->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	TreeNode->attachObject(Trees);

//	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
//	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(400.0, 400.0, 0.0));
//	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = Trees->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
//	Box = RobotEntity->getBoundingBox();
//	Center = Box.getCenter();

	m_Camera->setPosition(Ogre::Vector3(-1000.0, -1000.0, 0.0));
	m_Camera->lookAt(Ogre::Vector3(200.0, 200.0, 0.0));
}

void CAddingTreesView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CAddingTreesView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CAddingTreesView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -0.001 * (m_MousePosition.x - point.x);
	CameraMove[1] = 0.001 * m_MousePosition.y - point.y;

	CEngine * Engine = ((CAddingTreesApp*)AfxGetApp())->m_Engine;
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


BOOL CAddingTreesView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CAddingTreesApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
