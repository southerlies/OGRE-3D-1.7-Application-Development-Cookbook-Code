
// LandXmlView.cpp : implementation of the CLandXmlView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LandXmlApp.h"
#endif

#include "LandXmlDoc.h"
#include "LandXmlView.h"

//#include "LandXmlFace.h"
//#include "LandXML.h"
//#include "ILxNode.h"
#include "Document.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLandXmlView

IMPLEMENT_DYNCREATE(CLandXmlView, CView)

BEGIN_MESSAGE_MAP(CLandXmlView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLandXmlView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CLandXmlView construction/destruction

CLandXmlView::CLandXmlView()
{
	m_MouseNavigation = false;
}

CLandXmlView::~CLandXmlView()
{
}

BOOL CLandXmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLandXmlView drawing

void CLandXmlView::OnDraw(CDC* /*pDC*/)
{
	CLandXmlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CLandXmlView printing


void CLandXmlView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLandXmlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLandXmlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLandXmlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLandXmlView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLandXmlView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLandXmlView diagnostics

#ifdef _DEBUG
void CLandXmlView::AssertValid() const
{
	CView::AssertValid();
}

void CLandXmlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLandXmlDoc* CLandXmlView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLandXmlDoc)));
	return (CLandXmlDoc*)m_pDocument;
}
#endif //_DEBUG


// CLandXmlView message handlers


void CLandXmlView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CLandXmlDoc *Document = GetDocument();
	CEngine *Engine = ((CLandXmlApp*)AfxGetApp())->m_Engine;
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

void CLandXmlView::EngineSetup(void)
{
	Ogre::Root *Root = ((CLandXmlApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "LandXml");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("LandXml");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("LandXml", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setFarClipDistance(10000); 
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

	Ogre::ManualObject *Topography = GetManualObject("zigzag creek.xml", "Topography");
	Topography->setMaterialName(0, "Desert");
	Ogre::MeshPtr TopographyMesh = Topography->convertToMesh("Topography");
	Ogre::MeshSerializer MeshSerializer;
	MeshSerializer.exportMesh(TopographyMesh.getPointer(), "Topography.mesh");
	Ogre::SceneNode *Node = SceneManager->getRootSceneNode()->createChildSceneNode();

	Node->attachObject(Topography);
	
	Ogre::AxisAlignedBox Box = Topography->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();

	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode(Center);
	RobotNode->attachObject(RobotEntity);

	m_Camera->setPosition(Ogre::Vector3(Center[0], Center[1] + 100, Center[2]));
	m_Camera->lookAt(Center);
}

Ogre::ManualObject * CLandXmlView::GetManualObject(char* LandXmlPath, char* SurfaceName)
{
    USES_CONVERSION;
	Ogre::Root *Root = ((CLandXmlApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->getSceneManager("LandXml");
	Ogre::ManualObject *Topography = SceneManager->createManualObject("Topography");
	Topography->setDynamic(false);
    Topography->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	LX::Document* LxDocument = NULL;

	LX::ILxNode* LxRootNode = NULL;
    LX::LandXML* LandXml = NULL;	
	LX::Surfaces* Surfaces = NULL;
	LX::Surface* Surface = NULL;
	LX::Faces* Faces = NULL;
	LX::String Name(T2W(SurfaceName));

	LxDocument = LX::createDocumentObject();
	LPWSTR Path = T2W(LandXmlPath);
	LxDocument->loadXml(Path);
	LxDocument->releaseDOMDocument();

	Surface = LxDocument->getGlobalObjects().getSurfaceCollection().findFirstMatch(Name);
	LX::FacesCollection& FacesCollection = Surface->getDefinition()->Faces();
	LX::Pnts* Points = Surface->getDefinition()->getPnts();

	LX::PCollection& PointsCollection = Points->P();
	LX::PCollectionIterator* PointsCollectionIterator = PointsCollection.iterator();

	LX::P* P = NULL;

	double x;
	double y;
	double z;

	double xb;
	double yb;
	double zb;

	unsigned int PrevId = 0;
//////////////////////////Points///////////////////////////////
	while (!PointsCollectionIterator->atEnd())
	{
		P = PointsCollectionIterator->current();
		unsigned int id = P->getId();

		LX::IndexedListValueCollection<double>& Coordinates = P->value();

		x = Coordinates[0];
		y = Coordinates[1];
		z = Coordinates[2]; 

		if (PrevId == 0)
		{
			xb = x;
			yb = y;
			zb = z;
		}

		for (PrevId; PrevId < id + 1; PrevId++)
		{
//			Topography->position((x - xb), (z - zb), -(y - yb));
			Topography->position((x - xb)/100, (y - yb)/100, (z - zb)/100);
		}
		
		PointsCollectionIterator->next();
	}

	PointsCollectionIterator->release();

	LX::FacesCollectionIterator* FacesCollectionIterator = FacesCollection.iterator();

	LX::FCollectionIterator* FCollectionIterator = NULL;
	LX::F* F = NULL;

	while (!FacesCollectionIterator->atEnd())
	{
		Faces = FacesCollectionIterator->current();
		LX::FCollection& FCollection = Faces->F();
		FCollectionIterator = FCollection.iterator();

		while(!FCollectionIterator->atEnd())
		{
			F = FCollectionIterator->current();
			
			LX::IndexedListValueCollection<int>& Verteces = F->value();

			Topography->triangle(Verteces[0], Verteces[1], Verteces[2]);
			FCollectionIterator->next();
		}

		FCollectionIterator->release();
		FacesCollectionIterator->next();
	}

	Topography->end();

	return Topography;
}

void CLandXmlView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	Ogre::Root *Root = ((CLandXmlApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CLandXmlView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.5 * zDelta;

	CEngine * Engine = ((CLandXmlApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CLandXmlView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -0.001 * (m_MousePosition.x - point.x);
	CameraMove[1] = 0.001 * m_MousePosition.y - point.y;

	CEngine * Engine = ((CLandXmlApp*)AfxGetApp())->m_Engine;
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


void CLandXmlView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CLandXmlView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}
