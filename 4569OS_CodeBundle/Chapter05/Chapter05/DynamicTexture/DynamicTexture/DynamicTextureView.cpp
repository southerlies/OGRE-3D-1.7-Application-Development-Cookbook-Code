
// DynamicTextureView.cpp : implementation of the CDynamicTextureView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DynamicTexture.h"
#endif

#include "DynamicTextureDoc.h"
#include "DynamicTextureView.h"
#include <Vfw.h>
//#include <Wdm.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamicTextureView

IMPLEMENT_DYNCREATE(CDynamicTextureView, CView)

BEGIN_MESSAGE_MAP(CDynamicTextureView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDynamicTextureView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()

	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_TVCONTROL_START, &CDynamicTextureView::OnTvcontrolStart)
	ON_COMMAND(ID_TVCONTROL_STOP, &CDynamicTextureView::OnTvcontrolStop)
	ON_COMMAND(ID_TVCONTROL_SHIFT, &CDynamicTextureView::OnTvcontrolShift)
END_MESSAGE_MAP()

// CDynamicTextureView construction/destruction

CDynamicTextureView::CDynamicTextureView()
	: m_FrameNumber(0)
{
	m_Shift = 0;
}

CDynamicTextureView::~CDynamicTextureView()
{
}

BOOL CDynamicTextureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDynamicTextureView drawing

void CDynamicTextureView::OnDraw(CDC* /*pDC*/)
{
	CDynamicTextureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDynamicTextureView printing


void CDynamicTextureView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDynamicTextureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDynamicTextureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDynamicTextureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDynamicTextureView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDynamicTextureView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDynamicTextureView diagnostics

#ifdef _DEBUG
void CDynamicTextureView::AssertValid() const
{
	CView::AssertValid();
}

void CDynamicTextureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynamicTextureDoc* CDynamicTextureView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynamicTextureDoc)));
	return (CDynamicTextureDoc*)m_pDocument;
}
#endif //_DEBUG


// CDynamicTextureView message handlers
void CDynamicTextureView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CDynamicTextureDoc *Document = GetDocument();
	CEngine *Engine = ((CDynamicTextureApp*)AfxGetApp())->m_Engine;
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


void CDynamicTextureView::EngineSetup(void)
{
	Ogre::Root *Root = ((CDynamicTextureApp*)AfxGetApp())->m_Engine->GetRoot();

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
	m_Camera->setPosition(Ogre::Vector3(320.0, 240.0, 500.0));
	Ogre::SceneNode *CameraNode = NULL;
	CameraNode = SceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");

	Ogre::Viewport* Viewport = NULL;
	
	if (0 == m_RenderWindow->getNumViewports())
	{
		Viewport = m_RenderWindow->addViewport(m_Camera);
		Viewport->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	}

    // Alter the camera aspect ratio to match the viewport
    m_Camera->setAspectRatio(Ogre::Real(rect.Width()) / Ogre::Real(rect.Height()));
		
	Ogre::TexturePtr ScreenTexture = Ogre::TextureManager::getSingleton().createManual("Screen", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, 640, 480, 0, Ogre::PF_R8G8B8A8, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

	PixelBuffer = ScreenTexture->getBuffer();

	Ogre::ManualObject *Screen = SceneManager->createManualObject("Screen");
	Screen->setDynamic(true);
	Screen->begin("Screen", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(640,0,0);
	Screen->textureCoord(1,0);

	Screen->position(640,480,0);
	Screen->textureCoord(1,1);

	Screen->triangle(0, 1, 2);
		
	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(640,480,0);
	Screen->textureCoord(1,1);

	Screen->position(0,480,0);
	Screen->textureCoord(0,1);

	Screen->triangle(3, 4, 5);
	 
	Screen->end();

	Ogre::SceneNode* node = SceneManager->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0, 0, 0);
	node->attachObject(Screen);
		
	AVIFileInit();
	AVIFileOpen(&aviFile, "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\video\\crusades.avi",OF_READ,NULL);

	AVIFileGetStream(aviFile,&aviStream,streamtypeVIDEO,0);
	AVIFileRelease(aviFile);
	AVIStreamInfo(aviStream,&aviStreamInfo,sizeof(aviStreamInfo));
	BITMAPINFOHEADER bmpInfo;

	memset(&bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo.biSize = sizeof(BITMAPINFOHEADER) ; 
	bmpInfo.biBitCount = 32; 
	bmpInfo.biCompression = BI_RGB;
	bmpInfo.biHeight = 480;
	bmpInfo.biWidth = 640;
	bmpInfo.biPlanes = 1;
	bmpInfo.biSizeImage = 0;

	Frame = AVIStreamGetFrameOpen(aviStream, &bmpInfo); 

	Root->renderOneFrame();
}


void CDynamicTextureView::OnTimer(UINT_PTR nIDEvent)
{
	if (m_FrameNumber == aviStreamInfo.dwLength)
	{
		KillTimer(1);
		return;
	}

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(Frame, ++m_FrameNumber);
	LPVOID GetFrame = AVIStreamGetFrame(Frame, ++m_FrameNumber);

	if (lpbi == NULL)
	{
		KillTimer(1);
		return;
	}

	PixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	memcpy(PixelBuffer->getCurrentLock().data, lpbi + lpbi->biSize + 25, lpbi->biSizeImage);
	PixelBuffer->unlock();

	Ogre::Root *Root = ((CDynamicTextureApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnTimer(nIDEvent);
}


BOOL CDynamicTextureView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CDynamicTextureApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CDynamicTextureView::OnTvcontrolStart()
{
	SetTimer(1, 1, 0);
}


void CDynamicTextureView::OnTvcontrolStop()
{
	KillTimer(1);
}


void CDynamicTextureView::OnTvcontrolShift()
{
	m_Shift++;
}
