
// SpeechView.cpp : implementation of the CSpeechView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Speech.h"
#endif

#include "SpeechDoc.h"
#include "SpeechView.h"
#include "concrt.h"

#include <sphelper.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpeechView

IMPLEMENT_DYNCREATE(CSpeechView, CView)

BEGIN_MESSAGE_MAP(CSpeechView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpeechView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSpeechView construction/destruction

CSpeechView::CSpeechView()
{
	// TODO: add construction code here

}

CSpeechView::~CSpeechView()
{
}

BOOL CSpeechView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSpeechView drawing

void CSpeechView::OnDraw(CDC* /*pDC*/)
{
	CSpeechDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSpeechView printing


void CSpeechView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSpeechView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSpeechView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSpeechView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSpeechView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSpeechView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSpeechView diagnostics
void CSpeechView::EngineSetup(void)
{
	Ogre::Root *Root = ((CSpeechApp*)AfxGetApp())->m_Engine->GetRoot();
	Ogre::SceneManager *SceneManager = NULL;
	SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "MFCOgre");
    
	CComPtr<ISpVoice> Voice = ((CSpeechApp*)AfxGetApp())->m_cpVoice;
	CComPtr<ISpObjectToken> cpVoiceToken;
	CComPtr<IEnumSpObjectTokens> cpEnum;
	ULONG ulCount = 0;

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
	Ogre::MeshPtr HeadMesh = Ogre::MeshManager::getSingleton().load("facial.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

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

	 m_AnimationState = m_HeadEntity->getAnimationState("Speak");
	 m_AnimationState->setLoop(false);
	 m_AnimationState->setEnabled(true);
	 SetTimer(1, 10, 0);

	 //Enumerate the available voices 
	
	 SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

	//Get the number of voices
     cpEnum->GetCount(&ulCount);

	// Obtain a list of available voice tokens, set the voice to the token, and call Speak
	 while (ulCount -- )
	 {
		cpVoiceToken.Release();
        cpEnum->Next( 1, &cpVoiceToken, NULL );
		Voice->SetVoice(cpVoiceToken);
        Voice->Speak( L"How are you?<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL); 
	 }

	 Voice->Speak(L"This is the demo of the text to speech in the Ogre application<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Voice state control
	 Voice->Speak(L"This is the demo of the voice state control<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"This is the demo of the volume control<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Volume of voice control
	 Voice->Speak(L"<volume level = '50'>This text should be spoken at volume level fifty</volume><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<volume level = '100'>This text should be spoken at volume level one hundred</volume><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Rate of voice control
	 Voice->Speak(L"This is the demo of the rate control<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<rate absspeed = '-10'>This text should be spoken at rate negative ten</rate><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<rate absspeed = '10'>This text should be spoken at rate zero</rate><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<rate absspeed = '10'>This text should be spoken at rate ten</rate><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Pitch of voice control
	 Voice->Speak(L"This is the demo of the pitch control<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<pitch absmiddle = '-10'>This text should be spoken at pitch negative ten</pitch><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<pitch absmiddle = '0'>This text should be spoken at rate zero</pitch><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
	 Voice->Speak(L"<pitch absmiddle = '10'>This text should be spoken at rate ten</pitch><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Emphasis
	  Voice->Speak(L"The following word should be emphasized<emph>boo</emph>!<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Spelling
	  Voice->Speak(L"<spell>These words should be spelled out</spell>!<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Silence
	  Voice->Speak(L"Five hundred millseconds of silence<silence msec='500'/>just occured<silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);
//Pronunciation
  	  Voice->Speak(L"<pron sym='P r o n u n c i a t i o n'/><silence msec='1000'/>", SPF_ASYNC | SPF_IS_XML, NULL);

  	  Voice->Speak(L"Thank you for your patience. Good bye!!!", SPF_ASYNC | SPF_IS_XML, NULL);
}

#ifdef _DEBUG
void CSpeechView::AssertValid() const
{
	CView::AssertValid();
}

void CSpeechView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpeechDoc* CSpeechView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpeechDoc)));
	return (CSpeechDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpeechView message handlers


void CSpeechView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	CSpeechDoc *Document = GetDocument();
	CEngine *Engine = ((CSpeechApp*)AfxGetApp())->m_Engine;
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


void CSpeechView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CSpeechApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
	  return;

	m_AnimationState->addTime(Ogre::Real(0.01));
	Ogre::Root *Root = Engine->GetRoot();
    Root->renderOneFrame();
}
