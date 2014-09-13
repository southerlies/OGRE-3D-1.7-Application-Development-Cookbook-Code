// CaricatureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Caricature.h"
#include "CaricatureDlg.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "winbmp.h"
#include "tiffdec.h"
#include "jpegdec.h"

void Rothwell(CWinBmp * Bmp);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaricatureDlg dialog

CCaricatureDlg::CCaricatureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaricatureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaricatureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaricatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaricatureDlg)
	DDX_Control(pDX, IDC_ORIGINAL, m_Original);
	DDX_Control(pDX, IDC_CARICATURE, m_Caricature);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaricatureDlg, CDialog)
	//{{AFX_MSG_MAP(CCaricatureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_MODIFY_EMBOSS, OnModifyEmboss)
	ON_COMMAND(ID_MODIFY_CONTOUR, OnModifyContour)
	ON_COMMAND(ID_MODIFY_MIRROR, OnModifyMirror)
	ON_COMMAND(ID_MODIFY_PICASSO, OnModifyPicasso)
	ON_COMMAND(ID_MODIFY_CONESHRINKS, OnModifyConeShrinks)
	ON_COMMAND(ID_MODIFY_STEREO, OnModifyStereo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaricatureDlg message handlers

BOOL CCaricatureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCaricatureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCaricatureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
     Draw();
     CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCaricatureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCaricatureDlg::OnFileOpen() 
{
 static char BASED_CODE Filter[] = "Jpeg Files (*.jpg)|*.jpg||";

 CFileDialog Dialog(TRUE, ".jpg", NULL, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, Filter);

 if (IDCANCEL == Dialog.DoModal())
	{
  	 return;
	}
 
 m_OriginalPath = Dialog.GetPathName(); 	

 CJPEGDecoder JpegDecoder;
 JpegDecoder.MakeBmpFromFile(m_OriginalPath, &m_OriginalBmp);
 m_CaricatureBmp = m_OriginalBmp;
 Draw();
}

void CCaricatureDlg::OnFileSave() 
{
 m_CaricaturePath = m_OriginalPath;
 m_CaricaturePath.Replace(".jpg",".bmp");
 m_CaricatureBmp.SaveAsBmp(m_CaricaturePath);
 m_CaricatureBmp.SaveAsBmp(m_CaricaturePath);
}

void CCaricatureDlg::OnHelpAbout() 
{
	
}

void CCaricatureDlg::Draw()
{
 CRect Rect;

 CDC *DC = NULL;

 DC = m_Original.GetDC();
 SetStretchBltMode(DC->GetSafeHdc(),HALFTONE);

 m_Original.GetClientRect(&Rect);
 m_OriginalBmp.StretchDraw(DC->GetSafeHdc(),0,0,Rect.Width(),Rect.Height(),SRCCOPY);
 m_Original.ReleaseDC(DC);

 DC = m_Caricature.GetDC();
 SetStretchBltMode(DC->GetSafeHdc(),HALFTONE);
 m_Caricature.GetClientRect(&Rect);
 m_CaricatureBmp.StretchDraw(DC->GetSafeHdc(),0,0,Rect.Width(),Rect.Height(),SRCCOPY);
 m_Caricature.ReleaseDC(DC);
}

void CCaricatureDlg::OnModifyEmboss() 
{
 int EmbossFilter[3][3]={{2,0,0},{0,-1,0},{0,0,-1}};
 int SumRed = 0;
 int SumGreen = 0;
 int SumBlue = 0;

 CWinBmp Bmp;
 Bmp = m_OriginalBmp;

 RGBAPIXEL Pixel;

 BYTE Alpha;
 BYTE Red;
 BYTE Green;
 BYTE Blue;
 BYTE Gray;

  for (int x = 0; x < Bmp.GetWidth(); x++)
	 for (int y = 0; y < Bmp.GetHeight(); y++)
	 {
      Pixel = Bmp.GetPixel(x, y); 
	  
	  Red = ((BYTE *)&Pixel)[RGBA_RED];
      Green = ((BYTE *)&Pixel)[RGBA_GREEN];
	  Blue = ((BYTE *)&Pixel)[RGBA_BLUE];
	  Alpha = ((BYTE *)&Pixel)[RGBA_ALPHA];
 
	  Gray = (Red + Blue + Green)/3;
      SetRGBAPixel(&Pixel, Gray, Gray, Gray, Alpha);

      Bmp.SetPixel(x, y, Pixel);
	 }

 for (int i = 1; i < Bmp.GetWidth()-1; i++)
	 for (int j = 1; j < Bmp.GetHeight()-1; j++)
	 {
      SumRed = 0;
					for(int k=0;k<3;k++)
					{
						for(int l=0;l<3;l++)
						{
	  Pixel = Bmp.GetPixel(i-1+k, j-1+l); 
	  
	  Red = ((BYTE *)&Pixel)[RGBA_RED];
      Green = ((BYTE *)&Pixel)[RGBA_GREEN];
	  Blue = ((BYTE *)&Pixel)[RGBA_BLUE];
	  Alpha = ((BYTE *)&Pixel)[RGBA_ALPHA];

							SumRed += Red * EmbossFilter[k][l];
						}
					}
                    SumRed += 128;

					if(SumRed>255)
						SumRed=255;
					if(SumRed<0)
						SumRed=0;

      SetRGBAPixel(&Pixel, SumRed, SumRed, SumRed, Alpha);

      m_CaricatureBmp.SetPixel(i, j, Pixel);
	 }

 Draw();
}

void CCaricatureDlg::OnModifyContour() 
{
 Rothwell(&m_CaricatureBmp);
 Draw();
}

void CCaricatureDlg::OnModifyMirror() 
{
 RGBAPIXEL Pixel;

 CWinBmp Bmp;
 Bmp = m_OriginalBmp;

 int Width = Bmp.GetWidth();

 for (int x = 0; x < Bmp.GetWidth(); x++)
	 for (int y = 0; y < Bmp.GetHeight(); y++)
	 {
      Pixel = Bmp.GetPixel(x, y); 
      m_CaricatureBmp.SetPixel(Width - x - 1, y, Pixel);
	 }	

 Draw();
}

void CCaricatureDlg::OnModifyPicasso() 
{
 RGBAPIXEL Pixel;

 CWinBmp Bmp;
 Bmp = m_OriginalBmp;

 int Width = Bmp.GetWidth();
 int x;
 int y;

 for (x = 0; x < Bmp.GetWidth()/2; x++)
	 for (y = 0; y < Bmp.GetHeight(); y++)
	 {
      Pixel = Bmp.GetPixel(x, y); 
      m_CaricatureBmp.SetPixel(Width - x - 1, y, Pixel);
	 }	
/*
 for (x = Bmp.GetWidth()/2; x < Bmp.GetWidth(); x++)
	 for (y = Bmp.GetHeight()/2; y < Bmp.GetHeight(); y++)
	 {
      Pixel = Bmp.GetPixel(x, y); 
      m_CaricatureBmp.SetPixel(Width - x - 1, y, Pixel);
	 }	
*/
 Draw();
}

double CCaricatureDlg::PolarAngle(CWinBmp *Bmp, int x, int y)
{
 return atan2(x - Bmp->GetWidth()/2, y - Bmp->GetHeight()/2);
}

double CCaricatureDlg::PolarRadius(CWinBmp *Bmp, int x, int y)
{
 double dx = x - Bmp->GetWidth()/2;
 double dy = y - Bmp->GetHeight()/2;

 return sqrt(dx*dx + dy*dy);
}


int CCaricatureDlg::DecartX(CWinBmp *Bmp, double Radius, double Angle)
{
 return Bmp->GetWidth()/2 + (int)(Radius * cos(Angle));
}

int CCaricatureDlg::DecartY(CWinBmp *Bmp, double Radius, double Angle)
{
 return Bmp->GetHeight()/2 + (int)(Radius * sin(Angle));
}

void CCaricatureDlg::OnModifyConeShrinks() 
{
 RGBAPIXEL Pixel;

 CWinBmp Bmp;
 Bmp = m_OriginalBmp;

 int Width = Bmp.GetWidth();
 int Height = Bmp.GetHeight();

 for (int x = 0; x < Width; x++)
	 for (int y = 0; y < Height; y++)
	 {
      Pixel = Bmp.GetPixel(x, y); 
	  double Radius = PolarRadius(&m_OriginalBmp, x, y);
      double Angle = PolarAngle(&m_OriginalBmp, x, y);
	  int newX = DecartX(&m_OriginalBmp, sqrt(Width * Radius / 2), Angle);
	  int newY = DecartY(&m_OriginalBmp, sqrt(Height * Radius / 2), Angle);
	  if (newX > 0 && newX < Width && newY > 0 && newY < Height)
        m_CaricatureBmp.SetPixel(newX, newY, Pixel);
	 }	

 Draw();
}

void CCaricatureDlg::OnModifyStereo() 
{
 m_OriginalBmp.MakeGrayscale();
 m_CaricatureBmp.MakeGrayscale();

 RGBAPIXEL Pixel;
 
 RGBAPIXEL Pixel1;
 RGBAPIXEL Pixel2;
 BYTE Alpha;
 BYTE Red;
 BYTE Green;
 BYTE Blue;

 CWinBmp Bmp;
 Bmp = m_OriginalBmp;

 int Width = Bmp.GetWidth();
 int Height = Bmp.GetHeight();

 for (int x = 0; x < Width; x++)
	 for (int y = 0; y < Height; y++)
	 {
      Pixel1 = m_OriginalBmp.GetPixel(x, y); 
      Pixel2 = m_CaricatureBmp.GetPixel(x, y); 

	  Red = ((BYTE *)&Pixel1)[RGBA_BLUE];
	  Blue = ((BYTE *)&Pixel2)[RGBA_BLUE];
	  Alpha = ((BYTE *)&Pixel1)[RGBA_ALPHA];
 
      SetRGBAPixel(&Pixel, Red, 0, Blue, Alpha);
      m_CaricatureBmp.SetPixel(x, y, Pixel);
	 }	


 Draw();
}
