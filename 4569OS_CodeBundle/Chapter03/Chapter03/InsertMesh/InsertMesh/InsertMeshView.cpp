
// InsertMeshView.cpp : implementation of the CInsertMeshView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "InsertMesh.h"
#endif

#include "InsertMeshDoc.h"
#include "InsertMeshView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInsertMeshView

IMPLEMENT_DYNCREATE(CInsertMeshView, CView)

BEGIN_MESSAGE_MAP(CInsertMeshView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CInsertMeshView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CInsertMeshView construction/destruction

CInsertMeshView::CInsertMeshView()
{
	// TODO: add construction code here

}

CInsertMeshView::~CInsertMeshView()
{
}

BOOL CInsertMeshView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CInsertMeshView drawing

void CInsertMeshView::OnDraw(CDC* /*pDC*/)
{
	CInsertMeshDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CInsertMeshView printing


void CInsertMeshView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CInsertMeshView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CInsertMeshView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CInsertMeshView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CInsertMeshView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CInsertMeshView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CInsertMeshView diagnostics

#ifdef _DEBUG
void CInsertMeshView::AssertValid() const
{
	CView::AssertValid();
}

void CInsertMeshView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInsertMeshDoc* CInsertMeshView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInsertMeshDoc)));
	return (CInsertMeshDoc*)m_pDocument;
}
#endif //_DEBUG


// CInsertMeshView message handlers


void CInsertMeshView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CInsertMeshDoc *Document = GetDocument();
	CEngine *Engine = ((CInsertMeshApp*)AfxGetApp())->m_Engine;
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

void CInsertMeshView::EngineSetup(void)
{
	Ogre::Root *Root = ((CInsertMeshApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "InsertMesh");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("InsertMesh");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("InsertMesh", rect.Width(), rect.Height(), false, &parms);
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
		Viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
        
    DelaunayIt();

	CEdge *Edge = NULL; 
	CVertex Vertex;
	bool OnEdge = true;

	Vertex.m_Coordinate[0] = 0.5 * (m_Triangulation.m_VertexCollection.m_Max.m_Coordinate[0] + m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0]);
	Vertex.m_Coordinate[1] = 0.5 * (m_Triangulation.m_VertexCollection.m_Max.m_Coordinate[1] + m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1]);
	Vertex.m_Coordinate[2] = 0.0;

	Edge = m_Triangulation.LocateTriangle(&Vertex, OnEdge);

	m_Triangulation.TriangleInterpolate(Edge->m_Origin, Edge->m_Destination, Edge->m_Left->m_Destination, &Vertex);

	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode(
			Ogre::Vector3(Vertex.m_Coordinate[0],
						  Vertex.m_Coordinate[1],
						  Vertex.m_Coordinate[2]));
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);
}

void CInsertMeshView::DelaunayIt(void)
{
	Ogre::ManualObject *Topography = NULL;
	int TriangleIndex = 0;

	Ogre::Root *Root = ((CInsertMeshApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->getSceneManager("InsertMesh");
	m_Topography = SceneManager->createManualObject("Topography");
	m_Topography->setDynamic(false);
    m_Topography->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
	m_Triangulation.VertexSerialize(CString("C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\bin\\debug\\xml\\California\\test2.xyz"), CArchive::load);
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
 
   m_Topography->position(Vertex1->m_Coordinate[0] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0], 
	   Vertex1->m_Coordinate[1] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1], 
	   (Vertex1->m_Coordinate[2] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[2])/100);
   m_Topography->position(Vertex2->m_Coordinate[0] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0], 
	   Vertex2->m_Coordinate[1] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1], 
	   (Vertex2->m_Coordinate[2] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[2])/100);
   m_Topography->position(Vertex3->m_Coordinate[0] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[0], 
	   Vertex3->m_Coordinate[1] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[1], 
	   (Vertex3->m_Coordinate[2] - this->m_Triangulation.m_VertexCollection.m_Min.m_Coordinate[2])/100);

   m_Topography->triangle(TriangleIndex * 3 + 0, TriangleIndex * 3 + 1, TriangleIndex * 3 + 2);
   TriangleIndex++;
	}

	m_Topography->end();
}
