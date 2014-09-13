
// VideoView.cpp : implementation of the CVideoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Video.h"
#endif

#include "AviFile.h"
#include "VideoDoc.h"
#include "VideoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoView

IMPLEMENT_DYNCREATE(CVideoView, CView)

BEGIN_MESSAGE_MAP(CVideoView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIDEO_START, &CVideoView::OnVideoStart)
	ON_COMMAND(ID_VIDEO_STOP, &CVideoView::OnVideoStop)
	ON_COMMAND(ID_ANIMATION_START, &CVideoView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CVideoView::OnAnimationStop)
END_MESSAGE_MAP()

// CVideoView construction/destruction

CVideoView::CVideoView()
{
	m_First = true;
	m_MouseNavigation = false;
//	m_avi = NULL;
}

CVideoView::~CVideoView()
{
}

BOOL CVideoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVideoView drawing

void CVideoView::OnDraw(CDC* /*pDC*/)
{
	CVideoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CVideoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVideoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVideoView diagnostics

#ifdef _DEBUG
void CVideoView::AssertValid() const
{
	CView::AssertValid();
}

void CVideoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVideoDoc* CVideoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoDoc)));
	return (CVideoDoc*)m_pDocument;
}
#endif //_DEBUG


// CVideoView message handlers


void CVideoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;
	CView::OnLButtonDown(nFlags, point);
}


void CVideoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CVideoView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CVideoApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return;
	Ogre::Vector3 OldPosition = m_Camera->getPosition();
	m_Camera->moveRelative(CameraMove);
		
	m_MousePosition = point;

	Root->renderOneFrame();
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CVideoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CVideoApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CVideoView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CVideoDoc *Document = GetDocument();
	CEngine *Engine = ((CVideoApp*)AfxGetApp())->m_Engine;
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
	// Do not call CView::OnPaint() for painting messages
}


void CVideoView::OnTimer(UINT_PTR nIDEvent)
{
	CPaintDC dc(this);
	int Left;
	int Top;

 	Ogre::PixelFormat pf = m_RenderWindow->suggestPixelFormat();
	m_RenderWindow->getMetrics(mWidth, mHeight, mColorDepth, Left, Top);

	data = OGRE_ALLOC_T(Ogre::uchar, mWidth * mHeight * Ogre::PixelUtil::getNumElemBytes(pf), Ogre::MEMCATEGORY_RENDERSYS);
	
	Ogre::PixelBox m_PixelBox(mWidth, mHeight, mColorDepth, pf, data); 
	
	switch (nIDEvent)
	{
		case 1:
			
		m_RenderWindow->copyContentsToMemory(m_PixelBox);
		AVIStreamWrite(aviStream, m_FrameNumber++, 1, m_PixelBox.data, m_PixelBox.getHeight() * m_PixelBox.getHeight() * 4, AVIIF_KEYFRAME, NULL, NULL);

			break;

		case 2:

			break;
	}

	CView::OnTimer(nIDEvent);
}

void CVideoView::EngineSetup(void)
{
	Ogre::Root *Root = ((CVideoApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MFCOgre");

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

	try
	{
		m_RenderWindow = Root->createRenderWindow("Ogre in MFC", rect.Width(), rect.Height(), false, &parms);
	}
    catch(...)
	{
		MessageBox("Cannot initialize\nCheck that graphic-card driver is up-to-date", "Initialize Render System", MB_OK | MB_ICONSTOP);
		exit(EXIT_SUCCESS);
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
		Viewport->setBackgroundColour(Ogre::ColourValue(0.8f, 1.0f, 0.8f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::Entity *RobotEntity = m_SceneManager->createEntity("Robot", "robot.mesh");
	Ogre::SceneNode *RobotNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	RobotNode->attachObject(RobotEntity);

	Ogre::AxisAlignedBox Box = RobotEntity->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();
	m_Camera->lookAt(Center);

	Root->renderOneFrame();
}



void CVideoView::OnVideoStart()
{
	AVIFileInit();
	AVIFileOpen(&aviFile, "output.avi",OF_WRITE | OF_CREATE,NULL);
			
	int Left;
	int Top;

	AVISTREAMINFO  psi;
	ZeroMemory(&psi,sizeof(psi));

    psi.fccType = streamtypeVIDEO;// stream type
    psi.fccHandler = 0; 
    psi.dwScale = 24;
    psi.dwRate = 1000;
    psi.dwSuggestedBufferSize = 0;//mWidth * mHeight * Ogre::PixelUtil::getNumElemBytes(pf);

	AVIFileCreateStream(aviFile,&aviStream,&psi);
	AVIFileRelease(aviFile);
	AVIStreamInfo(aviStream,&aviStreamInfo,sizeof(aviStreamInfo));
	BITMAPINFOHEADER bmpInfo;
		
	Ogre::PixelFormat pf = m_RenderWindow->suggestPixelFormat();

	memset(&bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo.biSize = sizeof(BITMAPINFOHEADER) ; 
	bmpInfo.biBitCount = 32; 
	bmpInfo.biCompression = BI_RGB;
	bmpInfo.biHeight = mHeight;
	bmpInfo.biWidth = mWidth;
	bmpInfo.biPlanes = 1;
	bmpInfo.biSizeImage = mWidth * mHeight * Ogre::PixelUtil::getNumElemBytes(pf);

//	Frame = AVIStreamGetFrameOpen(aviStream, &bmpInfo); 

	SetTimer(1, 1, 0);	
}


void CVideoView::OnVideoStop()
{
	KillTimer(1);
	AVIStreamRelease(aviStream);
	OGRE_FREE(data, Ogre::MEMCATEGORY_RENDERSYS);	
}


void CVideoView::OnAnimationStart()
{
	// TODO: Add your command handler code here
}


void CVideoView::OnAnimationStop()
{
	// TODO: Add your command handler code here
}
