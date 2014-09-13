
// WeatherControlView.cpp : implementation of the CWeatherControlView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WeatherControl.h"
#endif

#include "WeatherControlDoc.h"
#include "WeatherControlView.h"
//#include "WeatherControlDlg.h"
#include <sphelper.h>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWeatherControlView

IMPLEMENT_DYNCREATE(CWeatherControlView, CView)

BEGIN_MESSAGE_MAP(CWeatherControlView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWeatherControlView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_WEATHERCONTROL_WEATHERCONTROL, &CWeatherControlView::OnWeatherControlWeatherControl)
	ON_WM_PAINT()
	ON_COMMAND(ID_WEATHERCONTROL_RAIN, &CWeatherControlView::OnWeatherControlRain)
	ON_COMMAND(ID_WEATHERCONTROL_SNOW, &CWeatherControlView::OnWeatherControlSnow)
	ON_COMMAND(ID_WEATHERCONTROL_FOG, &CWeatherControlView::OnWeatherControlFog)
	ON_COMMAND(ID_WEATHERCONTROL_SKY, &CWeatherControlView::OnWeatherControlSky)
	ON_COMMAND(ID_WEATHERCONTROL_SUN, &CWeatherControlView::OnWeatherControlSun)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CWeatherControlView construction/destruction

CWeatherControlView::CWeatherControlView()
{
	m_First = true;

	m_SceneManager = NULL;

//	m_WeatherControlDlg = NULL;
	m_RainControlDlg = NULL;
	m_SnowControlDlg = NULL;
	m_SkyControlDlg = NULL;
	m_FogControlDlg = NULL;
	m_SunControlDlg = NULL;

	m_Rain = NULL;
}

CWeatherControlView::~CWeatherControlView()
{
}

BOOL CWeatherControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWeatherControlView drawing

void CWeatherControlView::OnDraw(CDC* /*pDC*/)
{
	CWeatherControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CWeatherControlView printing


void CWeatherControlView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWeatherControlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWeatherControlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWeatherControlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CWeatherControlView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWeatherControlView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWeatherControlView diagnostics

#ifdef _DEBUG
void CWeatherControlView::AssertValid() const
{
	CView::AssertValid();
}

void CWeatherControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWeatherControlDoc* CWeatherControlView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeatherControlDoc)));
	return (CWeatherControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeatherControlView message handlers

/*
void CWeatherControlView::OnWeatherControlWeatherControl()
{
	if (m_WeatherControlDlg == NULL)
	{
		m_WeatherControlDlg = new CWeatherControlDlg();
		m_WeatherControlDlg->Create(IDD_WEATHER_CONTROL);
	}

	m_WeatherControlDlg->ShowWindow(SW_SHOW);
}
*/


void CWeatherControlView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CWeatherControlDoc *Document = GetDocument();
	CEngine *Engine = ((CWeatherControlApp*)AfxGetApp())->m_Engine;
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


void CWeatherControlView::EngineSetup(void)
{
	Ogre::Root *Root = ((CWeatherControlApp*)AfxGetApp())->m_Engine->GetRoot();

	m_SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "WeatherControl");

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

	Ogre::RenderTarget *RenderWindow = Root->getRenderTarget("WeatherControl");

	if (RenderWindow == NULL)
	{
	try
	{
		m_RenderWindow = Root->createRenderWindow("WeatherControl", rect.Width(), rect.Height(), false, &parms);
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

}


void CWeatherControlView::OnWeatherControlRain()
{
	Ogre::SceneNode *RainNode = NULL;

	if (!m_SceneManager->hasParticleSystem("Rain"))
	{
		m_Rain = m_SceneManager->createParticleSystem("Rain", "Examples/Rain");

		if (m_Rain != NULL)
		{
			RainNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("RainNode");
     		RainNode->attachObject(m_Rain);
			m_Rain->setVisible(false);
		}
	}

	if (m_RainControlDlg == NULL)
	{
		m_RainControlDlg = new CRainControlDlg();
		m_RainControlDlg->Create(IDD_RAIN_CONTROL);
	}

	m_RainControlDlg->ShowWindow(SW_SHOW);
}


void CWeatherControlView::OnWeatherControlSnow()
{
	Ogre::SceneNode *SnowNode = NULL;

	if (!m_SceneManager->hasParticleSystem("Snow"))
	{
		m_Snow = m_SceneManager->createParticleSystem("Snow", "Examples/Snow");

		if (m_Snow != NULL)
		{
			SnowNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("SnowNode");
    		SnowNode->attachObject(m_Snow);
			m_Snow->setVisible(false);
		}
	}

	if (m_SnowControlDlg == NULL)
	{
		m_SnowControlDlg = new CSnowControlDlg();
		m_SnowControlDlg->Create(IDD_SNOW_CONTROL);
	}

	m_SnowControlDlg->ShowWindow(SW_SHOW);
}


void CWeatherControlView::OnWeatherControlFog()
{
	if (m_FogControlDlg == NULL)
	{
		m_FogControlDlg = new CFogControlDlg();
		m_FogControlDlg->Create(IDD_FOG_CONTROL);
	}

	m_FogControlDlg->ShowWindow(SW_SHOW);
}


void CWeatherControlView::OnWeatherControlSky()
{
	if (m_SkyControlDlg == NULL)
	{
		m_SkyControlDlg = new CSkyControlDlg();
		m_SkyControlDlg->Create(IDD_SKY_CONTROL);
	}

	m_SkyControlDlg->ShowWindow(SW_SHOW);
}


void CWeatherControlView::OnWeatherControlSun()
{
	Ogre::SceneNode *SunNode = NULL;

	if (!m_SceneManager->hasParticleSystem("Sun"))
	{
		m_Sun = m_SceneManager->createParticleSystem("Sun", "Space/Sun");

		if (m_Sun != NULL)
		{
			SunNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("SunNode");
    		SunNode->attachObject(m_Sun);
			m_Sun->setVisible(false);
		}
	}

	if (m_SunControlDlg == NULL)
	{
		m_SunControlDlg = new CSunControlDlg();
		m_SunControlDlg->Create(IDD_SUN_CONTROL);
	}

	m_SunControlDlg->ShowWindow(SW_SHOW);
}


void CWeatherControlView::OnTimer(UINT_PTR nIDEvent)
{
	CEngine *Engine = ((CWeatherControlApp*)AfxGetApp())->m_Engine;

	if (Engine == NULL)
	  return;

	Ogre::Root *Root = Engine->GetRoot();
	if (Root == NULL)
    {
		return;
	}

	TCHAR szPath[MAX_PATH];

	CString ApplicationName = "WeatherControl.exe";

	GetModuleFileName(NULL, szPath, MAX_PATH);
    Ogre::ConfigFile OgreConfigFile;
	CString SoundPath(szPath);

	SoundPath = SoundPath.Left(SoundPath.GetLength() - ApplicationName.GetLength());
	SoundPath += L"\\..\\..\\media\\sounds\\rain\\rain storm.wav";
	CWeatherControlApp* WeatherControlApp = (CWeatherControlApp*)AfxGetApp();
	CComPtr<ISpVoice> Voice = WeatherControlApp->m_cpVoice;
	CComPtr<ISpStream> cpWavStream;

	switch (nIDEvent)
	{
	case ID_RAIN_TIMER:

		if (m_RainControlDlg != NULL)
		{
			if (m_RainControlDlg->m_PlaySound && m_Rain->getVisible())
			{
				SPBindToFile(SoundPath, SPFM_OPEN_READONLY, &cpWavStream);
				Voice->Resume();
				Voice->SpeakStream(cpWavStream, SPF_ASYNC, NULL);
			}
			else
			{
				Voice->Pause();
			}
		}
       Root->renderOneFrame();

		break;

	case ID_SUN_TIMER:
       Root->renderOneFrame();

		break;

	case ID_SNOW_TIMER:
       Root->renderOneFrame();

		break;

	case ID_FOG_TIMER:
       Root->renderOneFrame();

		break;

	}

	CView::OnTimer(nIDEvent);
}


void CWeatherControlView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


void CWeatherControlView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}


void CWeatherControlView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
}


BOOL CWeatherControlView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CWeatherControlApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

