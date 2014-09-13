
// LoadSceneView.cpp : implementation of the CLoadSceneView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LoadScene.h"
#endif

#include "LoadSceneDoc.h"
#include "LoadSceneView.h"

#include <encoding.h>
#include <xmlwriter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MY_ENCODING "ISO-8859-1"

// CLoadSceneView

IMPLEMENT_DYNCREATE(CLoadSceneView, CView)

BEGIN_MESSAGE_MAP(CLoadSceneView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLoadSceneView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CLoadSceneView construction/destruction

CLoadSceneView::CLoadSceneView()
{
	m_SceneManager = NULL;
}

CLoadSceneView::~CLoadSceneView()
{
}

BOOL CLoadSceneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLoadSceneView drawing

void CLoadSceneView::OnDraw(CDC* /*pDC*/)
{
	CLoadSceneDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CLoadSceneView printing


void CLoadSceneView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLoadSceneView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLoadSceneView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLoadSceneView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLoadSceneView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLoadSceneView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLoadSceneView diagnostics

#ifdef _DEBUG
void CLoadSceneView::AssertValid() const
{
	CView::AssertValid();
}

void CLoadSceneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLoadSceneDoc* CLoadSceneView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLoadSceneDoc)));
	return (CLoadSceneDoc*)m_pDocument;
}
#endif //_DEBUG


// CLoadSceneView message handlers


void CLoadSceneView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CLoadSceneDoc *Document = GetDocument();
	CEngine *Engine = ((CLoadSceneApp*)AfxGetApp())->m_Engine;
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

void CLoadSceneView::EngineSetup(void)
{
	Ogre::Root *Root = ((CLoadSceneApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "LoadScene");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("LoadScene");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("LoadScene", rect.Width(), rect.Height(), false, &parms);
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
		Viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

    xmlDocPtr doc;
    xmlNode *root_element = NULL;

	LIBXML_TEST_VERSION

	doc = xmlReadFile("C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\Chapter03\\SaveScene\\SaveScene\\1.scene", MY_ENCODING, 0);
	root_element = xmlDocGetRootElement(doc);

	Traverse(root_element, m_SceneManager->getRootSceneNode());
	m_Camera->lookAt(Ogre::Vector3(5, 50, 0));
}

void CLoadSceneView::Traverse(xmlNode * XmlNode, Ogre::SceneNode * SceneNode)
{
    xmlNode *cur_node = NULL;
	Ogre::SceneNode *ChildNode = NULL;
	Ogre::Camera * Camera = NULL;

    for (cur_node = XmlNode; cur_node; cur_node = cur_node->next) 
	{
        if (cur_node->type == XML_ELEMENT_NODE) 
		{
			if (_mbscmp(cur_node->name, BAD_CAST "RootSceneNode") == 0)
			{
				Traverse(cur_node->children, SceneNode);
			}

			if (_mbscmp(cur_node->name, BAD_CAST "SceneNode") == 0)
			{
				ChildNode = SceneNode->createChildSceneNode();		
				Traverse(cur_node->children, ChildNode);
			}
			
			if (_mbscmp(cur_node->name, BAD_CAST "Entity") == 0)
			{
				Ogre::String EntityName((char *)cur_node->properties->children->content);
				Ogre::String MeshName((char *)cur_node->properties->next->children->content);

				Ogre::Entity *Entity = m_SceneManager->createEntity(EntityName, MeshName);
				SceneNode->attachObject(Entity);
			}
		}
    }
}
