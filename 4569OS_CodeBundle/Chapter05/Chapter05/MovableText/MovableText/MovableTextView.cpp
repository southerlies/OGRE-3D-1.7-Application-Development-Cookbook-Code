
// MovableTextView.cpp : implementation of the CMovableTextView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MovableText.h"
#endif

#include "MovableTextDoc.h"
#include "MovableTextView.h"
#include "OgremovableText.h"

//class MovableText;
//using namespace Ogre;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMovableTextView

IMPLEMENT_DYNCREATE(CMovableTextView, CView)

BEGIN_MESSAGE_MAP(CMovableTextView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMovableTextView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMovableTextView construction/destruction

CMovableTextView::CMovableTextView()
{
	// TODO: add construction code here

}

CMovableTextView::~CMovableTextView()
{
}

BOOL CMovableTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMovableTextView drawing

void CMovableTextView::OnDraw(CDC* /*pDC*/)
{
	CMovableTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMovableTextView printing


void CMovableTextView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMovableTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMovableTextView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMovableTextView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMovableTextView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMovableTextView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMovableTextView diagnostics

#ifdef _DEBUG
void CMovableTextView::AssertValid() const
{
	CView::AssertValid();
}

void CMovableTextView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMovableTextDoc* CMovableTextView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMovableTextDoc)));
	return (CMovableTextDoc*)m_pDocument;
}
#endif //_DEBUG


// CMovableTextView message handlers
void CMovableTextView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CMovableTextDoc *Document = GetDocument();
	CEngine *Engine = ((CMovableTextApp*)AfxGetApp())->m_Engine;
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


void CMovableTextView::EngineSetup(void)
{
	Ogre::Root *Root = ((CMovableTextApp*)AfxGetApp())->m_Engine->GetRoot();

	Ogre::SceneManager *SceneManager = NULL;

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MFCOgre");

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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Ogre in MFC");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Ogre in MFC", rect.Width(), rect.Height(), false, &parms);
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
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::Entity *RobotEntity1 = SceneManager->createEntity("Robot1", "robot.mesh");
	Ogre::SceneNode *RobotNode1 = SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode1->attachObject(RobotEntity1);

	Ogre::Entity *RobotEntity2 = SceneManager->createEntity("Robot2", "robot.mesh");
	Ogre::SceneNode *RobotNode2 = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(100, 100, 100));
	RobotNode2->attachObject(RobotEntity2);

	Ogre::Vector3 Center1 = RobotNode1->getPosition();
	Ogre::Vector3 Center2 = RobotNode2->getPosition();

	Ogre::ManualObject *Measure = SceneManager->createManualObject("Measure");
	Measure->begin("BumpyMetal", Ogre::RenderOperation::OT_LINE_LIST); 
	Measure->position(Center1); 
	Measure->position(Center2); 
	Measure->end(); 
			
	Ogre::SceneNode *MeasureNode = SceneManager->getRootSceneNode()->createChildSceneNode("Measure");
	MeasureNode->attachObject(Measure);

	Ogre::SceneNode *LabelNode1 = SceneManager->getRootSceneNode()->createChildSceneNode("Robot 1");
	Ogre::MovableText *Label1 = new Ogre::MovableText("Label 1", "Robot 1", "BlueHighway", 1.0, Ogre::ColourValue::Black);
	Label1->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE); 
	Label1->setColor(Ogre::ColourValue::Blue);
	Label1->setAdditionalHeight(2.0);
	Label1->setCastShadows(false);
	LabelNode1->attachObject(Label1);
	LabelNode1->setPosition(Center1);

	Ogre::SceneNode *LabelNode2 = SceneManager->getRootSceneNode()->createChildSceneNode("Robot 2");
	Ogre::MovableText *Label2 = new Ogre::MovableText("Label 2", "Robot 2", "BlueHighway", 1.0, Ogre::ColourValue::Black);
	Label2->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE); 
	Label2->setColor(Ogre::ColourValue::Blue);
	Label2->setAdditionalHeight(2.0);
	LabelNode2->attachObject(Label2);
	LabelNode2->setPosition(Center2);

	Ogre::Real Distance = Center2.distance(Center1);
	char Dimension[20];
	sprintf(Dimension ,"%.2f", Distance);
	Ogre::SceneNode *MeasureTextNode = SceneManager->getRootSceneNode()->createChildSceneNode("MeasureText");
	Ogre::MovableText *MeasureText = new Ogre::MovableText("Measure", Dimension, "BlueHighway", 1.0, Ogre::ColourValue::Black);
	MeasureText->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE); 
	MeasureText->setColor(Ogre::ColourValue::Blue);
	MeasureTextNode->attachObject(&MeasureText);

	MeasureTextNode->setPosition(0.5 * (Center1 + Center2));

	m_Camera->lookAt(0.5 * (Center1 + Center2));
	Root->renderOneFrame();
}


BOOL CMovableTextView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CMovableTextApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
