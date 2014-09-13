
// SaveSceneView.cpp : implementation of the CSaveSceneView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SaveScene.h"
#endif

#include "SaveSceneDoc.h"
#include "SaveSceneView.h"

#include <encoding.h>
#include <xmlwriter.h>

#define MY_ENCODING "ISO-8859-1"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSaveSceneView

IMPLEMENT_DYNCREATE(CSaveSceneView, CView)

BEGIN_MESSAGE_MAP(CSaveSceneView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSaveSceneView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CSaveSceneView construction/destruction

CSaveSceneView::CSaveSceneView()
{
	// TODO: add construction code here

}

CSaveSceneView::~CSaveSceneView()
{
}

BOOL CSaveSceneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSaveSceneView drawing

void CSaveSceneView::OnDraw(CDC* /*pDC*/)
{
	CSaveSceneDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSaveSceneView printing


void CSaveSceneView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSaveSceneView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSaveSceneView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSaveSceneView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSaveSceneView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSaveSceneView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSaveSceneView diagnostics

#ifdef _DEBUG
void CSaveSceneView::AssertValid() const
{
	CView::AssertValid();
}

void CSaveSceneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSaveSceneDoc* CSaveSceneView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSaveSceneDoc)));
	return (CSaveSceneDoc*)m_pDocument;
}
#endif //_DEBUG


// CSaveSceneView message handlers


void CSaveSceneView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CSaveSceneDoc *Document = GetDocument();
	CEngine *Engine = ((CSaveSceneApp*)AfxGetApp())->m_Engine;
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

void CSaveSceneView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSaveSceneApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "SaveScene");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("SaveScene");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("SaveScene", rect.Width(), rect.Height(), false, &parms);
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
	CameraNode->attachObject(m_Camera);

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);


	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	int rc;

    xmlDocPtr doc;
   // Create a new Xmlm_XmlWriter for DOM, with no compression.
    m_XmlWriter = xmlNewTextWriterDoc(&doc, 0);
   // Start the document with the xml default for the version,
   // encoding ISO 8859-1 and the default for the standalone
   // declaration.
    xmlTextWriterStartDocument(m_XmlWriter, NULL, MY_ENCODING, NULL);
	
	SceneExplore(SceneManager);
   
	xmlTextWriterEndDocument(m_XmlWriter);
	xmlFreeTextWriter(m_XmlWriter);
    xmlSaveFileEnc("1.scene", doc, MY_ENCODING);
    xmlFreeDoc(doc);
}

void CSaveSceneView::SceneExplore(Ogre::SceneManager * SceneManager)
{
	Ogre::SceneNode *RootSceneNode = SceneManager->getRootSceneNode();
	
	xmlTextWriterStartElement(m_XmlWriter, BAD_CAST "RootSceneNode");//start RootSceneNode

	SceneNodeExplore(RootSceneNode);

	xmlTextWriterEndElement(m_XmlWriter); //end RootSceneNode
}

void CSaveSceneView::SceneNodeExplore(Ogre::SceneNode *SceneNode)
{
	Ogre::Entity *Entity = NULL;
	Ogre::Camera *Camera = NULL;
	Ogre::Light *Light = NULL;
	Ogre::ParticleSystem *ParticleSystem = NULL;
	Ogre::ManualObject *ManualObject = NULL;
	Ogre::BillboardSet *BillboardSet = NULL;

	xmlTextWriterStartElement(m_XmlWriter, BAD_CAST "SceneNode");
	
	Ogre::String SceneNodeName = SceneNode->getName();

	xmlTextWriterWriteAttribute(m_XmlWriter, 
										BAD_CAST "SceneNodeName",
										BAD_CAST SceneNodeName.c_str());

	Ogre::SceneNode::ObjectIterator obji = SceneNode->getAttachedObjectIterator();

	while (obji.hasMoreElements())
	{
		Ogre::MovableObject* mobj = obji.getNext();
	
		Ogre::String Type = mobj->getMovableType();
	
		if (Type == "Entity")
		{
			Entity = (Ogre::Entity *)(mobj);
			Ogre::String EntityName = Entity->getName();
			xmlTextWriterStartElement(m_XmlWriter, BAD_CAST "Entity");
			xmlTextWriterWriteAttribute(m_XmlWriter, 
										BAD_CAST "EntityName",
										BAD_CAST EntityName.c_str());

			Ogre::MeshPtr Mesh = Entity->getMesh();
			Ogre::String MeshName = Mesh->getName();
			xmlTextWriterWriteAttribute(m_XmlWriter, 
										BAD_CAST "MeshName",
										BAD_CAST MeshName.c_str());
			xmlTextWriterEndElement(m_XmlWriter); 
		}
		
		if (Type == "Camera")
		{
			Camera = (Ogre::Camera *)(mobj);
			Ogre::String CameraName = Camera->getName();
			xmlTextWriterStartElement(m_XmlWriter, BAD_CAST "Camera");
			
			xmlTextWriterWriteAttribute(m_XmlWriter, 
										BAD_CAST "CameraName",
										BAD_CAST CameraName.c_str());
			
			Ogre::Vector3 CameraPosition = Camera->getPosition();
			
			xmlTextWriterWriteFormatAttribute(m_XmlWriter, 
										BAD_CAST "XPosition",
										"%f",CameraPosition.x);
			xmlTextWriterWriteFormatAttribute(m_XmlWriter, 
										BAD_CAST "YPosition",
										"%f",CameraPosition.y);
			xmlTextWriterWriteFormatAttribute(m_XmlWriter, 
										BAD_CAST "ZPosition",
										"%f",CameraPosition.z);

			Ogre::Vector3 CameraDirection = Camera->getDirection();

			xmlTextWriterWriteFormatAttribute(m_XmlWriter, 
										BAD_CAST "XDirection",
										"%f",CameraDirection.x);
			xmlTextWriterWriteFormatAttribute(m_XmlWriter, 
										BAD_CAST "YDirection",
										"%f",CameraDirection.y);
			xmlTextWriterWriteFormatAttribute(m_XmlWriter, 
										BAD_CAST "ZDirection",
										"%f",CameraDirection.z);

			xmlTextWriterEndElement(m_XmlWriter); 
		}

		if (Type == "Light")
		{
			Light = (Ogre::Light *)(mobj);
		}

		if (Type == "ParticleSystem")
		{
			ParticleSystem = (Ogre::ParticleSystem *)(mobj);
		}

		if (Type == "ManualObject")
		{
			ManualObject = (Ogre::ManualObject *)(mobj);
		}

		if (Type == "BillboardSet")
		{
			BillboardSet = (Ogre::BillboardSet *)(mobj);
		}

	}

	Ogre::Node::ChildNodeIterator nodei = SceneNode->getChildIterator();

	while (nodei.hasMoreElements())
	{
		Ogre::SceneNode* node = (Ogre::SceneNode*)(nodei.getNext());
			// Add this subnode and its children...
		SceneNodeExplore(node);
	}

	xmlTextWriterEndElement(m_XmlWriter); //end SceneNode
}
