
// KeyboardInputView.cpp : implementation of the CKeyboardInputView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KeyboardInput.h"
#endif

#include "KeyboardInputDoc.h"
#include "KeyboardInputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKeyboardInputView

IMPLEMENT_DYNCREATE(CKeyboardInputView, CView)

BEGIN_MESSAGE_MAP(CKeyboardInputView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKeyboardInputView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_HOTKEY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CKeyboardInputView construction/destruction

CKeyboardInputView::CKeyboardInputView()
	: m_WorkingTimer(0)
{
}

CKeyboardInputView::~CKeyboardInputView()
{
}

BOOL CKeyboardInputView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKeyboardInputView drawing

void CKeyboardInputView::OnDraw(CDC* /*pDC*/)
{
	CKeyboardInputDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CKeyboardInputView printing


void CKeyboardInputView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKeyboardInputView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKeyboardInputView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKeyboardInputView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKeyboardInputView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKeyboardInputView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKeyboardInputView diagnostics

#ifdef _DEBUG
void CKeyboardInputView::AssertValid() const
{
	CView::AssertValid();
}

void CKeyboardInputView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKeyboardInputDoc* CKeyboardInputView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKeyboardInputDoc)));
	return (CKeyboardInputDoc*)m_pDocument;
}
#endif //_DEBUG


// CKeyboardInputView message handlers
void CKeyboardInputView::EngineSetup(void)
{
	Ogre::Root *Root = ((CKeyboardInputApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "KeyboardInput");
 
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
	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("Keyboard Input");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("Keyboard Input", rect.Width(), rect.Height(), false, &parms);
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

	m_HeadEntity = SceneManager->createEntity("Head", "facial.mesh");
	Ogre::SceneNode *HeadNode = SceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, -10, 0));
	HeadNode->attachObject(m_HeadEntity);
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

	RegisterHotKey(this->m_hWnd, 1, MOD_CONTROL, 'I');  
	RegisterHotKey(this->m_hWnd, 2, MOD_CONTROL, 'O');  
}


void CKeyboardInputView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	CKeyboardInputDoc *Document = GetDocument();
	CEngine *Engine = ((CKeyboardInputApp*)AfxGetApp())->m_Engine;
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


void CKeyboardInputView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CKeyboardInputView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	Ogre::Vector3 CameraMove;

	switch (nHotKeyId)
	{
	case 1:

		CameraMove[0] = 0;
		CameraMove[1] = 0;
		CameraMove[2] = -10;

		break;
	case 2:

		CameraMove[0] = 0;
		CameraMove[1] = 0;
		CameraMove[2] = 10;

		break;
	}

	Ogre::Root *Root = ((CKeyboardInputApp*)AfxGetApp())->m_Engine->GetRoot();
	m_Camera->moveRelative(CameraMove);
	Root->renderOneFrame();

	CView::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CKeyboardInputView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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


void CKeyboardInputView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(m_WorkingTimer);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CKeyboardInputView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	Ogre::Root *Root = ((CKeyboardInputApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void CKeyboardInputView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnSysKeyUp(nChar, nRepCnt, nFlags);
}


void CKeyboardInputView::OnTimer(UINT_PTR nIDEvent)
{
	Ogre::Vector3 CameraMove;

	switch (nIDEvent)
	{
		case 1:

			CameraMove[0] = -10;
			CameraMove[1] = 0;
			CameraMove[2] = 0;

		break;

		case 2:

			CameraMove[0] = 0;
			CameraMove[1] = 10;
			CameraMove[2] = 0;

			break;

		case 3:

			CameraMove[0] = 10;
			CameraMove[1] = 0;
			CameraMove[2] = 0;

			break;

		case 4:

			CameraMove[0] = 0;
			CameraMove[1] = -10;
			CameraMove[2] = 0;

			break;
	}

	Ogre::Root *Root = ((CKeyboardInputApp*)AfxGetApp())->m_Engine->GetRoot();
	m_Camera->moveRelative(CameraMove);
	Root->renderOneFrame();
	
	CView::OnTimer(nIDEvent);
}
