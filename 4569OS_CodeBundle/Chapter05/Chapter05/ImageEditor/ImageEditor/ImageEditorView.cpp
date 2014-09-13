
// ImageEditorView.cpp : implementation of the CImageEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImageEditor.h"
#endif

#include "ImageEditorDoc.h"
#include "ImageEditorView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageEditorView

IMPLEMENT_DYNCREATE(CImageEditorView, CView)

BEGIN_MESSAGE_MAP(CImageEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CImageEditorView construction/destruction

CImageEditorView::CImageEditorView()
{
	// TODO: add construction code here

}

CImageEditorView::~CImageEditorView()
{
}

BOOL CImageEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CImageEditorView drawing

void CImageEditorView::OnDraw(CDC* /*pDC*/)
{
	CImageEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CImageEditorView printing


void CImageEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CImageEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageEditorView diagnostics

#ifdef _DEBUG
void CImageEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CImageEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageEditorDoc* CImageEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageEditorDoc)));
	return (CImageEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageEditorView message handlers
void CImageEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CImageEditorDoc *Document = GetDocument();
	CEngine *Engine = ((CImageEditorApp*)AfxGetApp())->m_Engine;
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


void CImageEditorView::EngineSetup(void)
{
	Ogre::Root *Root = ((CImageEditorApp*)AfxGetApp())->m_Engine->GetRoot();

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
	m_Camera->setPosition(Ogre::Vector3(444.0, 332.0, 100.0));
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
		
	Ogre::TexturePtr StereoTexture = Ogre::TextureManager::getSingleton().createManual("stereo", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, 888, 665, 0, Ogre::PF_R8G8B8A8, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

	Ogre::HardwarePixelBufferSharedPtr PixelBuffer = StereoTexture->getBuffer();
	int size = StereoTexture->getSize();
	Ogre::ManualObject *Screen = SceneManager->createManualObject("Screen");
	Screen->setDynamic(true);
	Screen->begin("stereo", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Screen->position(0,0,0);
	Screen->textureCoord(0,0);
		
	Screen->position(888,0,0);
	Screen->textureCoord(1,0);

	Screen->position(888,665,0);
	Screen->textureCoord(1,1);

	Screen->triangle(0, 1, 2);
		
	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(888,665,0);
	Screen->textureCoord(1,1);

	Screen->position(0,665,0);
	Screen->textureCoord(0,1);

	Screen->triangle(3, 4, 5);
	 
	Screen->end();

	Ogre::SceneNode *StereoNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	StereoNode->attachObject(Screen);

	Ogre::Image LoadImage;
	Ogre::Image LeftImage;
	Ogre::Image RightImage;

	LoadImage.load("Left.jpg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	LeftImage = LoadImage.flipAroundX();
	Ogre::PixelBox LeftBox = LeftImage.getPixelBox();

	int PixelIndex;
	BYTE *LeftData;
	LeftData = (BYTE *)LeftBox.data;

	BYTE Red;
	BYTE Green;
	BYTE Blue;
	BYTE Grey;
/**/
	for (PixelIndex = 0; PixelIndex < LeftBox.getConsecutiveSize()/3; PixelIndex++)
	{
		Red = LeftData[PixelIndex * 3 + 0];
		Green = LeftData[PixelIndex * 3 + 1];
		Blue = LeftData[PixelIndex * 3 + 2];

		Grey = Red * 0.212671 + Green * 0.715160 + Blue * 0.072169;

		LeftData[PixelIndex * 3 + 0] = Grey;
		LeftData[PixelIndex * 3 + 1] = Grey;
		LeftData[PixelIndex * 3 + 2] = Grey;
	}

	LoadImage.load("Right.jpg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	RightImage = LoadImage.flipAroundX();
	Ogre::PixelBox RightBox = RightImage.getPixelBox();

	BYTE *RightData;

	RightData = (BYTE *)RightBox.data;

	for (PixelIndex = 0; PixelIndex < RightBox.getConsecutiveSize()/3; PixelIndex++)
	{
		Red = RightData[PixelIndex * 3 + 0];
		Green = RightData[PixelIndex * 3 + 1];
		Blue = RightData[PixelIndex * 3 + 2];

		Grey = Red * 0.212671 + Green * 0.715160 + Blue * 0.072169;

		RightData[PixelIndex * 3 + 0] = Grey;
		RightData[PixelIndex * 3 + 1] = Grey;
		RightData[PixelIndex * 3 + 2] = Grey;
	}
	
	PixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	Ogre::PixelBox Destination = PixelBuffer->getCurrentLock();
	
	BYTE *DestData;

	DestData = (BYTE *)Destination.data;
	
	for (PixelIndex = 0; PixelIndex < LeftBox.getConsecutiveSize()/3; PixelIndex++)
	{
		LeftData[PixelIndex * 3 + 1] = 0;
		LeftData[PixelIndex * 3 + 2] = RightData[PixelIndex * 3 + 2];
	}
	
	Ogre::PixelUtil::bulkPixelConversion(LeftBox, Destination);	
	
	PixelBuffer->unlock();
}


BOOL CImageEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CImageEditorApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
