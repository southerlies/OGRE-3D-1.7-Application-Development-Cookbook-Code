
// SceneEditorView.cpp : implementation of the CSceneEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SceneEditor.h"
#endif

#include "SceneEditorDoc.h"
#include "SceneEditorView.h"
#include "ChildSceneNodeDlg.h"
#include "EntityCreatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSceneEditorView

IMPLEMENT_DYNCREATE(CSceneEditorView, CView)

BEGIN_MESSAGE_MAP(CSceneEditorView, CView)
	ON_WM_PAINT()
	ON_COMMAND(ID_EDITSCENE_SCENEMANAGER, &CSceneEditorView::OnEditSceneManager)
	ON_COMMAND(ID_EDITSCENE_ADDSCENENODE, &CSceneEditorView::OnEditsceneAddscenenode)
	ON_COMMAND(ID_EDITSCENE_ADDCAMERA, &CSceneEditorView::OnEditsceneAddcamera)
	ON_COMMAND(ID_EDITSCENE_ADDLIGHT, &CSceneEditorView::OnEditsceneAddlight)
	ON_COMMAND(ID_EDITSCENE_ADDPARTICLESYSTEM, &CSceneEditorView::OnEditsceneAddparticlesystem)
	ON_COMMAND(ID_EDITSCENE_ADDENTITY, &CSceneEditorView::OnEditsceneAddentity)
	ON_COMMAND(ID_EDITSCENE_DESTROY, &CSceneEditorView::OnEditsceneDestroy)
	ON_COMMAND(ID_EDITSCENE_DESTROYENTITIES, &CSceneEditorView::OnEditsceneDestroyentities)
END_MESSAGE_MAP()

// CSceneEditorView construction/destruction

CSceneEditorView::CSceneEditorView()
{
	m_Root = NULL;
	m_SceneManager = NULL;
	m_SceneManagerDlg = NULL;
}

CSceneEditorView::~CSceneEditorView()
{
}

BOOL CSceneEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSceneEditorView drawing

void CSceneEditorView::OnDraw(CDC* /*pDC*/)
{
	CSceneEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSceneEditorView diagnostics

#ifdef _DEBUG
void CSceneEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CSceneEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSceneEditorDoc* CSceneEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSceneEditorDoc)));
	return (CSceneEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CSceneEditorView message handlers


void CSceneEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CSceneEditorDoc *Document = GetDocument();
	CEngine *Engine = ((CSceneEditorApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
	  return;

	m_Root = Engine->GetRoot();

	if (m_First && m_Root != NULL)
    {
        m_First = false;
        EngineSetup();
    }

	if (m_Root != NULL)
    {
       m_Root->renderOneFrame();
	}

}

void CSceneEditorView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSceneEditorApp*)AfxGetApp())->m_Engine->GetRoot();
	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "SceneEditor");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("SceneEditor");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("SceneEditor", rect.Width(), rect.Height(), false, &parms);
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
/*
	Ogre::Entity *RobotEntity = SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);
*/
}

void CSceneEditorView::OnEditSceneManager()
{
	if (this->m_SceneManagerDlg == NULL)
	{
		m_SceneManagerDlg = new CSceneManagerDlg();
		m_SceneManagerDlg->Create(IDD_SCENE_MANAGER);
	}

	m_SceneManagerDlg->ShowWindow(SW_SHOW);
}


void CSceneEditorView::OnEditsceneAddscenenode()
{
	CChildSceneNodeDlg ChildSceneNodeDlg;
	
	if (IDOK == ChildSceneNodeDlg.DoModal())
	{
		HTREEITEM Selected = m_SceneManagerDlg->m_SceneTree.GetSelectedItem();
		m_SceneManagerDlg->m_SceneTree.InsertItem(ChildSceneNodeDlg.m_NodeName, Selected);
		m_SceneManagerDlg->m_SceneTree.Expand(Selected, TVE_EXPAND);
		m_SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::String(ChildSceneNodeDlg.m_NodeName));
		
		if (m_Root != NULL)
		{
			m_Root->renderOneFrame();
		}
	}
}


void CSceneEditorView::OnEditsceneAddcamera()
{
	// TODO: Add your command handler code here
}


void CSceneEditorView::OnEditsceneAddlight()
{
	// TODO: Add your command handler code here
}


void CSceneEditorView::OnEditsceneAddparticlesystem()
{
	// TODO: Add your command handler code here
}


void CSceneEditorView::OnEditsceneAddentity()
{
	CEntityCreatorDlg EntityCreatorDlg;

	if (IDOK == EntityCreatorDlg.DoModal())
	{
		HTREEITEM Selected = m_SceneManagerDlg->m_SceneTree.GetSelectedItem();
		m_SceneManagerDlg->m_SceneTree.InsertItem(EntityCreatorDlg.m_EntityName, Selected);
		
		Ogre::String SceneNodeName = m_SceneManagerDlg->m_SceneTree.GetItemText(Selected);
		Ogre::Entity *Entity = m_SceneManager->createEntity(Ogre::String(EntityCreatorDlg.m_EntityName), Ogre::String(EntityCreatorDlg.m_MeshName));
		Ogre::SceneNode *SceneNode = m_SceneManager->getSceneNode(SceneNodeName);
		SceneNode->attachObject(Entity);
	
		Ogre::AxisAlignedBox Box = Entity->getBoundingBox();
		Ogre::Vector3 Center = Box.getCenter();
		m_Camera->lookAt(Center);

		m_SceneManagerDlg->m_SceneTree.Expand(Selected, TVE_EXPAND);
		
		if (m_Root != NULL)
		{
			m_Root->renderOneFrame();
		}
	}
}


void CSceneEditorView::OnEditsceneDestroy()
{
	// TODO: Add your command handler code here
}


void CSceneEditorView::OnEditsceneDestroyentities()
{
	// TODO: Add your command handler code here
}
