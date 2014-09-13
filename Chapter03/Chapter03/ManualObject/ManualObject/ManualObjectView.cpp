
// ManualObjectView.cpp : implementation of the CManualObjectView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ManualObject.h"
#endif

#include "ManualObjectDoc.h"
#include "ManualObjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Ogre::String Materials[] = 
{
	"Examples/SphereMappedRustySteel",
	"Examples/OgreLogo",
	"Examples/BeachStones",
	"Examples/TrippySkyBox",
	"Examples/SpaceSkyBox",
	"Examples/DynamicCubeMap",
	"Examples/RustySteel",
	"Examples/Chrome",
	"Examples/WaterStream",
	"Examples/Flare"
};
// CManualObjectView

IMPLEMENT_DYNCREATE(CManualObjectView, CView)

BEGIN_MESSAGE_MAP(CManualObjectView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_COMMAND(ID_ACTIONS_MATERIALS, &CManualObjectView::OnActionsMaterials)
	ON_COMMAND(ID_ACTIONS_CHANGEOPERATIONS, &CManualObjectView::OnActionsChangeoperations)
	ON_COMMAND(ID_ACTIONS_RESET, &CManualObjectView::OnActionsReset)
END_MESSAGE_MAP()

// CManualObjectView construction/destruction

CManualObjectView::CManualObjectView()
{
	m_ManualObject = NULL;
	m_SceneManager = NULL;
}

CManualObjectView::~CManualObjectView()
{
}

BOOL CManualObjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CManualObjectView drawing

void CManualObjectView::OnDraw(CDC* /*pDC*/)
{
	CManualObjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CManualObjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CManualObjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CManualObjectView diagnostics

#ifdef _DEBUG
void CManualObjectView::AssertValid() const
{
	CView::AssertValid();
}

void CManualObjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CManualObjectDoc* CManualObjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CManualObjectDoc)));
	return (CManualObjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CManualObjectView message handlers


void CManualObjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CManualObjectDoc *Document = GetDocument();
	CEngine *Engine = ((CManualObjectApp*)AfxGetApp())->m_Engine;
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

void CManualObjectView::EngineSetup(void)
{
	Ogre::Root *Root = ((CManualObjectApp*)AfxGetApp())->m_Engine->GetRoot();
	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "m_ManualObject");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("m_ManualObject");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("m_ManualObject", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->lookAt(0.0, 0.0, 0.0);

	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));

	CreateManualObject();
	Ogre::AxisAlignedBox Box = m_ManualObject->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);
	m_Camera->setPosition(300, 100, 200);

//	m_Camera->setPolygonMode(Ogre::PM_WIREFRAME);
	Root->renderOneFrame();
}

void CManualObjectView::CreateManualObject(void)
{
	FILE *fp;
	fp = fopen("C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\stl\\Tubes\\hoses\\curvature\\1_4.stl", "r");

	while(getc(fp) != '\n') { }
	
	float x;
	float y;
	float z;
	
	float nx;
	float ny;
	float nz;

	int TriangleIndex = 0;

	m_MaterialIndex = 0;
	m_ManualObject = m_SceneManager->createManualObject("stl");
	m_ManualObject->setDynamic(true);
	m_ManualObject->begin(Materials[m_MaterialIndex++], Ogre::RenderOperation::OT_TRIANGLE_LIST);

	while (!feof(fp))
    {
 	   int ret = fscanf(fp, "%*s %*s %f %f %f\n", &nx, &ny, &nz); //facet normal nx ny nz
			
      ret = fscanf(fp, "%*s %*s"); //outer loop
      ret = fscanf(fp, "%*s %f %f %f\n", &x,  &y,  &z); //vertex x y z
	  
	  m_ManualObject->position(x, y, z);
	  m_ManualObject->normal(nx, ny, nz);
	  m_ManualObject->colour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
      ret=fscanf(fp, "%*s %f %f %f\n", &x,  &y,  &z); //vertex x y z
		
	  m_ManualObject->position(x, y, z);
	  m_ManualObject->normal(nx, ny, nz);
	  m_ManualObject->colour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	  ret=fscanf(fp, "%*s %f %f %f\n", &x,  &y,  &z); //vertex x y z
		
	  m_ManualObject->position(x, y, z);
	  m_ManualObject->normal(nx, ny, nz);
	  m_ManualObject->colour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));

	  m_ManualObject->triangle(TriangleIndex * 3 + 0, TriangleIndex * 3 + 1, TriangleIndex * 3 + 2);
	  TriangleIndex++;

	  ret=fscanf(fp, "%*s"); //endloop
      ret=fscanf(fp, "%*s"); //endfacet
      
	  if (feof(fp)) 
		break;

	  if (TriangleIndex % 1000 == 0)
	  {
		  m_ManualObject->end();
		  m_ManualObject->begin(Materials[m_MaterialIndex++], Ogre::RenderOperation::OT_TRIANGLE_LIST);
		  TriangleIndex = 0;
	  }
    }
  
	fclose(fp);

	m_ManualObject->end();

	Ogre::SceneNode *SceneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	SceneNode->attachObject(m_ManualObject);
}


void CManualObjectView::OnActionsMaterials()
{
	for (int Index = 0; Index < m_ManualObject->getNumSections(); Index++)
	{
		m_ManualObject->setMaterialName(Index, Materials[m_ManualObject->getNumSections() - Index]);
	}

	Ogre::Root *Root = ((CManualObjectApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();
}


void CManualObjectView::OnActionsChangeoperations()
{
	// TODO: Add your command handler code here
}


void CManualObjectView::OnActionsReset()
{
	for (int Index = 0; Index < m_ManualObject->getNumSections(); Index++)
	{
		m_ManualObject->setMaterialName(Index, Materials[Index]);
	}

	Ogre::Root *Root = ((CManualObjectApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();
}
