
// DelaunayView.cpp : implementation of the CDelaunayView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Delaunay.h"
#endif

#include "DelaunayDoc.h"
#include "DelaunayView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDelaunayView

IMPLEMENT_DYNCREATE(CDelaunayView, CView)

BEGIN_MESSAGE_MAP(CDelaunayView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSCHAR()
END_MESSAGE_MAP()

// CDelaunayView construction/destruction

CDelaunayView::CDelaunayView()
{
	m_MouseNavigation = false;
}

CDelaunayView::~CDelaunayView()
{
}

BOOL CDelaunayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDelaunayView drawing

void CDelaunayView::OnDraw(CDC* /*pDC*/)
{
	CDelaunayDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CDelaunayView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDelaunayView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDelaunayView diagnostics

#ifdef _DEBUG
void CDelaunayView::AssertValid() const
{
	CView::AssertValid();
}

void CDelaunayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDelaunayDoc* CDelaunayView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDelaunayDoc)));
	return (CDelaunayDoc*)m_pDocument;
}
#endif //_DEBUG


// CDelaunayView message handlers


void CDelaunayView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDelaunayDoc *Document = GetDocument();
	CEngine *Engine = ((CDelaunayApp*)AfxGetApp())->m_Engine;
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

void CDelaunayView::EngineSetup(void)
{
	Ogre::Root *Root = ((CDelaunayApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Delaunay");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Delaunay");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Delaunay", rect.Width(), rect.Height(), false, &parms);
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
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *Node = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(626145.0,4539495.0,1459.992));
	Node->attachObject(RobotEntity);
	DelaunayIt();

	Node->attachObject(m_Topography);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	
	m_Camera->lookAt(Center);
//	Center[2] += 100;
//	m_Camera->setPosition(Center);
//	m_Camera->moveRelative(Ogre::Vector3(0.0, 0.0, 500.0));
	//m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);
//	SceneManager->showBoundingBoxes(true);
}

void CDelaunayView::DelaunayIt(void)
{
	Ogre::ManualObject *Topography = NULL;
	int TriangleIndex = 0;

	Ogre::Root *Root = ((CDelaunayApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->getSceneManager("Delaunay");
	m_Topography = SceneManager->createManualObject("Topography");
	m_Topography->setDynamic(false);
    m_Topography->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
	m_Triangulation.VertexSerialize(CString("C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\bin\\debug\\xml\\California\\test1.xyz"), CArchive::load);
	m_Triangulation.m_VertexNumber = 0;
	m_Triangulation.m_VertexCollection.m_VertexNumber = 0;
	m_Triangulation.DelaunayIt();
    
//	CVertex *Vertex = NULL;

//	for (long VertexIndex = 4; VertexIndex < m_Triangulation.m_VertexCollection.m_Collection.GetCount(); VertexIndex++)
//	{
//		Vertex = (CVertex *)m_Triangulation.m_VertexCollection.m_Collection[VertexIndex];
//		m_Topography->position(Vertex->m_Coordinate[0],Vertex->m_Coordinate[1], Vertex->m_Coordinate[2]);
//	}

 CEdge *Edge1 = NULL;
 CEdge *Edge2 = NULL;
 CEdge *Edge3 = NULL;

 CVertex Verteces[3];
 CVertex *Vertex1 = NULL;
 CVertex *Vertex2 = NULL;
 CVertex *Vertex3 = NULL;

 int InsideCounter;

 m_Triangulation.ClearPrinted();

 int VertexIndex = 0;
 int FaceIndex = 0;

 for (long int EdgeIndex = 0; EdgeIndex < m_Triangulation.m_EdgeCollection.m_Collection.GetCount(); EdgeIndex++)
 {
  Edge1 = (CEdge *) m_Triangulation.m_EdgeCollection.m_Collection[EdgeIndex];
 
  if (Edge1 == NULL)
	  continue;

  if (Edge1->m_Printed)
     continue;

  Vertex1 = Edge1->m_Origin;
  
  if (Vertex1->m_Index < 4)
  {
   continue;
  }

  Edge2 = Edge1->m_Left;
  
  if (Edge2 == NULL)
    continue;

  if (Edge2->m_Printed)
     continue;

   Vertex2 = Edge2->m_Origin;

  if (Vertex2->m_Index < 4)
   {
    continue;
   }

   Edge3 = Edge2->m_Left;

   if (Edge3 == NULL)
    continue;

   if (Edge3->m_Printed)
     continue;

   Vertex3 = Edge3->m_Origin;

  if (Vertex3->m_Index < 4)
   {
	   continue;
   }

   if (Edge1->m_Inside)
   {
	    continue;
   }

  if (Edge2->m_Inside)
  {
	   continue;
  }

   if (Edge3->m_Inside)
   {
	   continue;
   }

/*
   CVertex::VertexType Type1 = Vertex1->m_VertexType;
   CVertex::VertexType Type2 = Vertex2->m_VertexType;
   CVertex::VertexType Type3 = Vertex3->m_VertexType;

   if (Type1 == CVertex::Shoulder || Type1 == CVertex::Hole)
  {
    continue;
  }

   if (Type2 == CVertex::Shoulder || Type2 == CVertex::Hole)
  {
    continue;
  }

   if (Type3 == CVertex::Shoulder || Type3 == CVertex::Hole)
  {
    continue;
  }
*/
   Edge1->m_Printed = true;
   Edge2->m_Printed = true;
   Edge3->m_Printed = true;
 
   m_Topography->position(
	   Vertex1->m_Coordinate[0] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0], 
	   Vertex1->m_Coordinate[1] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1], 
	   Vertex1->m_Coordinate[2] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[2]
   );
   m_Topography->position(
	   Vertex2->m_Coordinate[0] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0], 
	   Vertex2->m_Coordinate[1] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1], 
	   Vertex2->m_Coordinate[2] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[2]
	   );
   m_Topography->position(
	   Vertex3->m_Coordinate[0] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0], 
	   Vertex3->m_Coordinate[1] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1], 
	   Vertex3->m_Coordinate[2] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[2]
	   );

   m_Topography->triangle(TriangleIndex * 3 + 0, TriangleIndex * 3 + 1, TriangleIndex * 3 + 2);
   TriangleIndex++;
	}

	m_Topography->end();
}


void CDelaunayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CDelaunayView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CDelaunayView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -0.001 * (m_MousePosition.x - point.x);
	CameraMove[1] = 0.001 * m_MousePosition.y - point.y;

	CEngine * Engine = ((CDelaunayApp*)AfxGetApp())->m_Engine;
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


BOOL CDelaunayView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.05 * zDelta;

	CEngine * Engine = ((CDelaunayApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CDelaunayView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'W':

		m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);

		break;

	case 'S':

		m_Camera->setPolygonMode(Ogre::PM_SOLID);

		break;

	case 'P':

		m_Camera->setPolygonMode(Ogre::PM_POINTS);

		break;
	}

	Ogre::Root *Root = ((CDelaunayApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void CDelaunayView::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnSysChar(nChar, nRepCnt, nFlags);
}
