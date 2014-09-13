
// DynamicGrammarView.cpp : implementation of the CDynamicGrammarView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DynamicGrammar.h"
#endif

#include "DynamicGrammarDoc.h"
#include "DynamicGrammarView.h"
#include <sphelper.h>
#include "Expression.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamicGrammarView

IMPLEMENT_DYNCREATE(CDynamicGrammarView, CView)

BEGIN_MESSAGE_MAP(CDynamicGrammarView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDynamicGrammarView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_MESSAGE(WM_RECOEVENT, &CDynamicGrammarView::OnRecoEvent)

END_MESSAGE_MAP()

// CDynamicGrammarView construction/destruction

CDynamicGrammarView::CDynamicGrammarView()
{
	// TODO: add construction code here

}

CDynamicGrammarView::~CDynamicGrammarView()
{
}

BOOL CDynamicGrammarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDynamicGrammarView drawing

void CDynamicGrammarView::OnDraw(CDC* /*pDC*/)
{
	CDynamicGrammarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDynamicGrammarView printing


void CDynamicGrammarView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDynamicGrammarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDynamicGrammarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDynamicGrammarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDynamicGrammarView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDynamicGrammarView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDynamicGrammarView diagnostics

#ifdef _DEBUG
void CDynamicGrammarView::AssertValid() const
{
	CView::AssertValid();
}

void CDynamicGrammarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynamicGrammarDoc* CDynamicGrammarView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynamicGrammarDoc)));
	return (CDynamicGrammarDoc*)m_pDocument;
}
#endif //_DEBUG


void CDynamicGrammarView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CDynamicGrammarDoc *Document = GetDocument();
	CEngine *Engine = ((CDynamicGrammarApp*)AfxGetApp())->m_Engine;
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


afx_msg LRESULT CDynamicGrammarView::OnRecoEvent(WPARAM wParam, LPARAM lParam)
{
    CSpEvent event;  
	
    // Loop processing events while there are any in the queue
    while (event.GetFrom(((CDynamicGrammarApp*)AfxGetApp())->m_cpRecoCtxt) == S_OK)
    {
        // Look at recognition event only
        switch (event.eEventId)
        {
		case SPEI_RECOGNITION:
			ExecuteCommand(event.RecoResult());
			break;
        }
    }

	return 0;
}


void CDynamicGrammarView::ExecuteCommand(ISpPhrase * Phrase)
{
    SPPHRASE *pElements;
	Ogre::Root *Root = ((CDynamicGrammarApp*)AfxGetApp())->m_Engine->GetRoot();

 	if (SUCCEEDED(Phrase->GetPhrase(&pElements)))
    {        
        switch (pElements->Rule.ulId)
        {
	      case VID_ExpressionType:
            {
				const SPPHRASEPROPERTY *pProp = pElements->pProperties;

				while (pProp) 
				{
					m_ManualKeyFrame->updatePoseReference(pProp->vValue.ulVal, 1.0);
					m_ManualAnimState->getParent()->_notifyDirty();
					Root->renderOneFrame();

					pProp = pProp->pNextSibling;
				}
            } 
			
			break;
        }

		// Free the pElements memory which was allocated for us
        ::CoTaskMemFree(pElements);
    }
}


void CDynamicGrammarView::EngineSetup(void)
{
	Ogre::Root *Root = ((CDynamicGrammarApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "Dynamic Grammar");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Dynamic Grammar");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Dynamic Grammar", rect.Width(), rect.Height(), false, &parms);
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
//	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 100.0));

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
	Ogre::MeshPtr m_HeadMesh = Ogre::MeshManager::getSingleton().load("facial.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	m_ManualKeyFrame = m_HeadMesh->createAnimation("Manual", 0)->createVertexTrack(4, Ogre::VAT_POSE)->createVertexPoseKeyFrame(0);
		// create pose references for the first 15 poses
	for (unsigned int i = 0; i < 15; i++) 
	{
		m_ManualKeyFrame->addPoseReference(i, 0);
	}
	m_HeadEntity = SceneManager->createEntity("Head", "facial.mesh");
	Ogre::SceneNode *HeadNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, -10, 0));
	HeadNode->attachObject(m_HeadEntity);
	m_ManualAnimState = m_HeadEntity->getAnimationState("Manual");
	m_ManualAnimState->setEnabled(true);

	SPSTATEHANDLE hDynamicRuleHandle;
	
	((CDynamicGrammarApp*)AfxGetApp())->m_cpCmdGrammar->GetRule(L"VID_ExpressionType", NULL, SPRAF_Dynamic, FALSE, &hDynamicRuleHandle);
	((CDynamicGrammarApp*)AfxGetApp())->m_cpCmdGrammar->ClearRule(hDynamicRuleHandle);
	((CDynamicGrammarApp*)AfxGetApp())->m_cpCmdGrammar->Commit(0);

	for (unsigned int i = 0; i < m_ManualKeyFrame->getPoseReferences().size(); i++)
	{
		Ogre::String poseName = m_HeadMesh->getPose(i)->getName();
		CSpDynamicString ds(poseName.c_str());
		
		SPPROPERTYINFO prop;
		prop.pszName = L"Id";
		prop.pszValue = L"Property";
		prop.vValue.vt = VT_I4;
		prop.vValue.ulVal = i;

		((CDynamicGrammarApp*)AfxGetApp())->m_cpCmdGrammar->AddWordTransition(hDynamicRuleHandle, NULL, ds , L" -.",
			SPWT_LEXICAL, 1.0, &prop);
	}

	((CDynamicGrammarApp*)AfxGetApp())->m_cpCmdGrammar->Commit(0);
	((CDynamicGrammarApp*)AfxGetApp())->m_cpCmdGrammar->SetRuleIdState(VID_ExpressionType, SPRS_ACTIVE);

	Ogre::AxisAlignedBox Box = m_HeadEntity->getBoundingBox();
	Ogre::Vector3 HeadCenter = Box.getCenter();
	m_Camera->lookAt(HeadCenter);
	m_Camera->setAutoTracking(true, HeadNode);
	m_Camera->setFixedYawAxis(true);
	m_Camera->yaw(Ogre::Radian(0));
	m_Camera->pitch(Ogre::Radian(0));
	m_Camera->setPosition(HeadNode->_getDerivedPosition());
	m_Camera->setOrientation(HeadNode->_getDerivedOrientation());
	m_Camera->moveRelative(Ogre::Vector3(0, 0, 180));
}



