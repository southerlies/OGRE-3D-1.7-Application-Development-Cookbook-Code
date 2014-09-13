
// VideoInTextureView.cpp : implementation of the CVideoInTextureView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "VideoInTexture.h"
#endif

#include "VideoInTextureDoc.h"
#include "VideoInTextureView.h"
#include <Vfw.h>
#include "VideoTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoInTextureView

IMPLEMENT_DYNCREATE(CVideoInTextureView, CView)

BEGIN_MESSAGE_MAP(CVideoInTextureView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_COMMAND(ID_VIDEO_START, &CVideoInTextureView::OnVideoStart)
	ON_COMMAND(ID_VIDEO_STOP, &CVideoInTextureView::OnVideoStop)
END_MESSAGE_MAP()

// CVideoInTextureView construction/destruction

CVideoInTextureView::CVideoInTextureView()
{
	m_First = true;
	m_MouseNavigation = false;
  
}

CVideoInTextureView::~CVideoInTextureView()
{
}

BOOL CVideoInTextureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVideoInTextureView drawing

void CVideoInTextureView::OnDraw(CDC* /*pDC*/)
{
	CVideoInTextureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CVideoInTextureView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVideoInTextureView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVideoInTextureView diagnostics

#ifdef _DEBUG
void CVideoInTextureView::AssertValid() const
{
	CView::AssertValid();
}

void CVideoInTextureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVideoInTextureDoc* CVideoInTextureView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoInTextureDoc)));
	return (CVideoInTextureDoc*)m_pDocument;
}
#endif //_DEBUG


// CVideoInTextureView message handlers


void CVideoInTextureView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CVideoInTextureApp*)AfxGetApp())->m_Engine;
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


BOOL CVideoInTextureView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CVideoInTextureApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CVideoInTextureView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MousePosition = point;
	m_MouseNavigation = true;

	CView::OnLButtonDown(nFlags, point);
}


void CVideoInTextureView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CVideoInTextureView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CVideoInTextureDoc *Document = GetDocument();
	CEngine *Engine = ((CVideoInTextureApp*)AfxGetApp())->m_Engine;
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

void CVideoInTextureView::EngineSetup(void)
{
	Ogre::Root *Root = ((CVideoInTextureApp*)AfxGetApp())->m_Engine->GetRoot();

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
	m_Camera->setPosition(Ogre::Vector3(320.0, 240.0, 500.0));
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
		
	m_Camera->lookAt(320.0, 240.0, 0.0);

	m_VideoTexture = new CVideoTexture(m_SceneManager, 640.0, 480.0, "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\video\\crusades.avi");
	Root->renderOneFrame();
}


void CVideoInTextureView::OnTimer(UINT_PTR nIDEvent)
{
	if (m_VideoTexture->m_FrameNumber == m_VideoTexture->m_aviStreamInfo.dwLength)
	{
		KillTimer(1);
		return;
	}

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_VideoTexture->m_Frame, ++m_VideoTexture->m_FrameNumber);
	LPVOID GetFrame = AVIStreamGetFrame(m_VideoTexture->m_Frame, ++m_VideoTexture->m_FrameNumber);

	if (lpbi == NULL)
	{
		KillTimer(1);
		return;
	}

	m_VideoTexture->m_PixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	memcpy(m_VideoTexture->m_PixelBuffer->getCurrentLock().data, lpbi + lpbi->biSize + 25, lpbi->biSizeImage);
	m_VideoTexture->m_PixelBuffer->unlock();

	Ogre::Root *Root = ((CVideoInTextureApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}



void CVideoInTextureView::OnVideoStart()
{
	SetTimer(1, 1, 0);
}


void CVideoInTextureView::OnVideoStop()
{
	KillTimer(1);
}
