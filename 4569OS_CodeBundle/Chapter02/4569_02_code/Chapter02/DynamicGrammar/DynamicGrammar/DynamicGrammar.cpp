
// DynamicGrammar.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DynamicGrammar.h"
#include "MainFrm.h"

#include "DynamicGrammarDoc.h"
#include "DynamicGrammarView.h"

#include <sphelper.h>
#include "Expression.h"
#include "Engine.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamicGrammarApp

BEGIN_MESSAGE_MAP(CDynamicGrammarApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDynamicGrammarApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDynamicGrammarApp construction

CDynamicGrammarApp::CDynamicGrammarApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DynamicGrammar.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CDynamicGrammarApp object

CDynamicGrammarApp theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

// {12FA5758-39B2-4FF6-B41B-9258F7BA27E7}
static const CLSID clsid =
{ 0x12FA5758, 0x39B2, 0x4FF6, { 0xB4, 0x1B, 0x92, 0x58, 0xF7, 0xBA, 0x27, 0xE7 } };

const GUID CDECL _tlid = { 0xBBD4DBE9, 0xDA53, 0x42F2, { 0xBF, 0x41, 0x60, 0x42, 0x6B, 0x58, 0x5B, 0xA0 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CDynamicGrammarApp initialization

BOOL CDynamicGrammarApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Packt Publishing\\Ogre3D Cookbook\\Dynamic Grammar Application"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDynamicGrammarDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDynamicGrammarView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Connect the COleTemplateServer to the document template
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// Note: SDI applications register server objects only if /Embedding
		//   or /Automation is present on the command line


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register all OLE server factories as running.  This enables the
		//  OLE libraries to create objects from other applications
		COleTemplateServer::RegisterAll();

		// Don't show the main window
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.  Unregister
	// typelibrary.  Other unregistration occurs in ProcessShellCommand().
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		UnregisterShellFileTypes();
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	InitializeSpeechRecognition();
	InitEngine();
	
	return TRUE;
}

int CDynamicGrammarApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CDynamicGrammarApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CDynamicGrammarApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDynamicGrammarApp customization load/save methods

void CDynamicGrammarApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CDynamicGrammarApp::LoadCustomState()
{
}

void CDynamicGrammarApp::SaveCustomState()
{
}

// CDynamicGrammarApp message handlers


bool CDynamicGrammarApp::InitializeSpeechRecognition(void)
{
	if (FAILED(CoInitialize(NULL)))
	{
		AfxMessageBox("Error starting COM");
		return false;
	}

	HRESULT hRes = m_cpEngine.CoCreateInstance(CLSID_SpSharedRecognizer);

	if (FAILED(hRes))
	{
		AfxMessageBox("Error starting SAPI");
		return false;
	}

	hRes = m_cpEngine->CreateRecoContext(&m_cpRecoCtxt);

	if (FAILED(hRes))
	{
		AfxMessageBox("Error creating context");
		return false;
	}

	hRes = m_cpRecoCtxt->SetNotifyWindowMessage(m_pMainWnd->m_hWnd, WM_RECOEVENT, 0, 0);

	if (FAILED(hRes))
	{
		AfxMessageBox("Error creating notification window");
		return false;
	}

	hRes = m_cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));

	if (FAILED(hRes))
	{
		AfxMessageBox("Error creating interest");
		return false;
	}

	hRes = m_cpRecoCtxt->CreateGrammar(0, &m_cpCmdGrammar);
	
	if (FAILED(hRes)) 
	{
		AfxMessageBox("Error creating grammar");
		return false;
	}

	hRes = m_cpCmdGrammar->LoadCmdFromResource(
		NULL,
		MAKEINTRESOURCEW(IDR_EXPRESSION),
		L"SRGRAMMAR",
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), 
		SPLO_DYNAMIC);
	
	if (FAILED(hRes)) 
	{
		AfxMessageBox("Error creating grammar rules");
		return false;
	}

	// Set rules to active, we are now listening for commands

	hRes = m_cpCmdGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE );
	
	if (FAILED(hRes)) 
	{
		AfxMessageBox("Error setting rule state");
		return false;
	}

	return true;
}


void CDynamicGrammarApp::InitEngine(void)
{
	m_Engine = new CEngine();
}
