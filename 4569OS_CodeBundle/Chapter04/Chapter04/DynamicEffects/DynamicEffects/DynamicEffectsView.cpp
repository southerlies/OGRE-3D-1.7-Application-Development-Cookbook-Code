
// DynamicEffectsView.cpp : implementation of the CDynamicEffectsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DynamicEffects.h"
#endif

#include "DynamicEffectsDoc.h"
#include "DynamicEffectsView.h"
#include "Cone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamicEffectsView

IMPLEMENT_DYNCREATE(CDynamicEffectsView, CView)

BEGIN_MESSAGE_MAP(CDynamicEffectsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDynamicEffectsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EFFECTSCONTROL_EFFECTSCONTROL, &CDynamicEffectsView::OnEffectsControlEffectsControl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_HOTKEY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CDynamicEffectsView construction/destruction

CDynamicEffectsView::CDynamicEffectsView()
{
	m_MouseNavigation = false;
	m_First = true;
	m_DynamicEffectsControlDlg = NULL;
}

CDynamicEffectsView::~CDynamicEffectsView()
{
}

BOOL CDynamicEffectsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDynamicEffectsView drawing

void CDynamicEffectsView::OnDraw(CDC* /*pDC*/)
{
	CDynamicEffectsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDynamicEffectsView printing


void CDynamicEffectsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDynamicEffectsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDynamicEffectsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDynamicEffectsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDynamicEffectsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDynamicEffectsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDynamicEffectsView diagnostics

#ifdef _DEBUG
void CDynamicEffectsView::AssertValid() const
{
	CView::AssertValid();
}

void CDynamicEffectsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynamicEffectsDoc* CDynamicEffectsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynamicEffectsDoc)));
	return (CDynamicEffectsDoc*)m_pDocument;
}
#endif //_DEBUG


// CDynamicEffectsView message handlers


void CDynamicEffectsView::OnEffectsControlEffectsControl()
{
	if (m_DynamicEffectsControlDlg == NULL)
	{
		m_DynamicEffectsControlDlg = new CDynamicEffectsControlDlg();
		m_DynamicEffectsControlDlg->Create(IDD_DYNAMIC_EFFECTS);
	}

	m_DynamicEffectsControlDlg->ShowWindow(SW_SHOW);}


void CDynamicEffectsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CDynamicEffectsDoc *Document = GetDocument();
	CEngine *Engine = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine;
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


void CDynamicEffectsView::EngineSetup(void)
{
	Ogre::Root *Root = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine->GetRoot();

	srand( (unsigned)time( NULL ) );
	Ogre::SceneManager *SceneManager = NULL;

	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "DynamicEffects");

	SceneManager->setAmbientLight(Ogre::ColourValue::Black);
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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("DynamicEffects");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("DynamicEffects", rect.Width(), rect.Height(), false, &parms);
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
	m_Camera->setPosition(Ogre::Vector3(200.0, 50.0, 10.0));
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
		
	Ogre::AxisAlignedBox Box(-1000, -1000, -1000, 1000, 1000, 1000);
	Ogre::Vector3 Center = Box.getCenter();

	Light = SceneManager->createLight(); 

	m_Camera->setPosition(Ogre::Vector3(25.0, 25.0, 25.0));
	m_Camera->setDirection((Ogre::Vector3(0.0, 100.0, 0.0) - m_Camera->getPosition()).normalisedCopy());
	Light->setDirection((Ogre::Vector3(0.0, 100.0, 0.0) - Ogre::Vector3(0.0, 0.0, 0.0)).normalisedCopy());

	Ogre::Real Intensity = m_Camera->getDirection().dotProduct(Light->getDirection());

	CCone ConeObject;
	Cone = ConeObject.CreateCone(Intensity);
	Ogre::SceneNode *ConeNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0.0, 100.0, 0.0));
	ConeNode->attachObject(Cone);

	Ogre::SceneNode* lightNode = SceneManager->getRootSceneNode()->createChildSceneNode(Center);

		Light->setType(Ogre::Light::LT_SPOTLIGHT);            
		Light->setVisible(true);
		Light->setPosition(Ogre::Vector3(0.0, 100.0, 0.0));
		Light->setSpotlightOuterAngle(Ogre::Radian(0.4));
		Light->setDiffuseColour(Intensity, Intensity, 0.0);      
		Light->setSpecularColour(Intensity, Intensity, 0.0);    

		lightNode->attachObject(Light);
	
}


void CDynamicEffectsView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine;
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


BOOL CDynamicEffectsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CDynamicEffectsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CDynamicEffectsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CDynamicEffectsView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CDynamicEffectsApp*)AfxGetApp())->m_Engine;

	if (Engine == NULL)
	  return;

	Ogre::Root *Root = Engine->GetRoot();
	if (Root == NULL)
    {
		return;
	}
	Ogre::Vector3 CameraMove;

	switch (nIDEvent)
	{
		case 1:

			CameraMove[0] = -1;
			CameraMove[1] = 0;
			CameraMove[2] = 0;

		break;

		case 2:

			CameraMove[0] = 0;
			CameraMove[1] = 1;
			CameraMove[2] = 0;

			break;

		case 3:

			CameraMove[0] = 1;
			CameraMove[1] = 0;
			CameraMove[2] = 0;

			break;

		case 4:

			CameraMove[0] = 0;
			CameraMove[1] = -1;
			CameraMove[2] = 0;

			break;
	}

	m_Camera->moveRelative(CameraMove);
	m_Camera->setDirection((Light->getPosition() - m_Camera->getPosition()).normalisedCopy());

	Ogre::Real Intensity = m_Camera->getDirection().dotProduct(Light->getDirection());

	Light->setDiffuseColour(Intensity, Intensity, 0.0);      
	Light->setSpecularColour(Intensity, Intensity, 0.0); 

/////////////////////////////////////////////////
	int numSegBase = 24;
	int numSegHeight = 24;
	Ogre::Real radius = 10.0;
	Ogre::Real height = 20.0;

    Cone->beginUpdate(0);

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;
	
	Ogre::Real uTile = 1.0;
	Ogre::Real vTile = 1.0;

	Ogre::Vector3 refNormal = Ogre::Vector3(radius, height, 0.f).normalisedCopy();
	Ogre::Quaternion q;
	int offset = 0;

	for (int i = 0; i <=numSegHeight; i++)
	{
		Ogre::Real r0 = radius * (1 - i / (Ogre::Real)numSegHeight);
		for (int j = 0; j<=numSegBase; j++)
		{
			Ogre::Real x0 = r0* cosf(j * deltaAngle);
			Ogre::Real z0 = r0 * sinf(j * deltaAngle);
			Cone->position(x0, i * deltaHeight, z0);
			Cone->colour(Intensity, Intensity, 0.0, 0.0);
			q.FromAngleAxis(Ogre::Radian(-j*deltaAngle), Ogre::Vector3::NEGATIVE_UNIT_Y);
			Cone->normal(q*refNormal);
			Cone->textureCoord(j / (Ogre::Real)numSegBase * uTile, i / (Ogre::Real)numSegHeight * vTile);

			if (i != numSegHeight&& j != numSegBase)
			{
				Cone->index(offset + numSegBase + 2);
				Cone->index(offset);
				Cone->index(offset + numSegBase + 1);
				Cone->index(offset + numSegBase + 2);
				Cone->index(offset + 1);
				Cone->index(offset);
			}

			offset ++;
		}
	}
/**/	
	//low cap
	int centerIndex = offset;
	
	Cone->position(0,0,0);
	Cone->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
	Cone->textureCoord(0.0,vTile);
	offset++;
	for (int j=0; j<=numSegBase; j++)
	{
		Ogre::Real x0 = radius * cosf(j*deltaAngle);
		Ogre::Real z0 = radius * sinf(j*deltaAngle);

		Cone->position(x0, 0.0f, z0);
		Cone->colour(Intensity, Intensity, 0.0, 0.0);
		Cone->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		Cone->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);
		if (j!=numSegBase)
		{
			Cone->index(centerIndex);
			Cone->index(offset);
			Cone->index(offset+1);
		}
		offset++;
	}
/**/
	Cone->end();
///////////////////////////////////////////////////
	Root->renderOneFrame();	

	CView::OnTimer(nIDEvent);
}


void CDynamicEffectsView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CDynamicEffectsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(m_WorkingTimer);
	m_WorkingTimer = 0;

	switch (nChar)
	{
		case VK_LEFT: //left
		case 65: //A
		case 97: //a

			m_WorkingTimer = 1;

			break;

		case VK_UP:  //up
		case 87:  //W
		case 119: //w

			m_WorkingTimer = 2;

			break;

		case VK_RIGHT: //right
		case 68: //D
		case 100: //d

			m_WorkingTimer = 3;

			break;

		case VK_DOWN: //down
		case 83: //S
		case 115://s

			m_WorkingTimer = 4;

			break;
	}

	if (m_WorkingTimer != 0)
	  SetTimer(m_WorkingTimer, 10, NULL);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDynamicEffectsView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(m_WorkingTimer);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
