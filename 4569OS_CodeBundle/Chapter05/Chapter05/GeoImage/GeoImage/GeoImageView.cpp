
// GeoImageView.cpp : implementation of the CGeoImageView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GeoImage.h"
#endif

#include "GeoImageDoc.h"
#include "GeoImageView.h"

#include "geotiffio.h"
#include "geotiff.h"
#include "tiffio.h"
#include "xtiffio.h"
#include "geo_normalize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeoImageView

IMPLEMENT_DYNCREATE(CGeoImageView, CView)

BEGIN_MESSAGE_MAP(CGeoImageView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSKEYDOWN()
END_MESSAGE_MAP()

// CGeoImageView construction/destruction

CGeoImageView::CGeoImageView()
{
	m_MouseNavigation = false;
}

CGeoImageView::~CGeoImageView()
{
}

BOOL CGeoImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGeoImageView drawing

void CGeoImageView::OnDraw(CDC* /*pDC*/)
{
	CGeoImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CGeoImageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGeoImageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGeoImageView diagnostics

#ifdef _DEBUG
void CGeoImageView::AssertValid() const
{
	CView::AssertValid();
}

void CGeoImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGeoImageDoc* CGeoImageView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeoImageDoc)));
	return (CGeoImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CGeoImageView message handlers
void CGeoImageView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
	CGeoImageDoc *Document = GetDocument();
	CEngine *Engine = ((CGeoImageApp*)AfxGetApp())->m_Engine;
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


void CGeoImageView::EngineSetup(void)
{
	Ogre::Root *Root = ((CGeoImageApp*)AfxGetApp())->m_Engine->GetRoot();

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
//	m_Camera->setPosition(Ogre::Vector3(320.0, 240.0, 500.0));
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
		

	TIFF 	*Tif = (TIFF*)0;  /* TIFF-level descriptor */
  	GTIF	*GTif = (GTIF*)0; /* GeoKey-level descriptor */

	CString SourcePath = "C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\materials\\textures\\o41078a1.tif";
 int ImageWidth;
 int ImageHeight;

 double LowerLeftX;
 double LowerLeftY;

 double UpperRightX;
 double UpperRightY;

 int Xpos;
 int Ypos;

	Tif = XTIFFOpen((LPCSTR)SourcePath, "r");
	GTif = GTIFNew(Tif);
 
	GTIFDefn Definition;
	GTIFGetDefn(GTif, &Definition);
 
	GTIFPrint(GTif, 0, 0);

	TIFFGetField( Tif, TIFFTAG_IMAGEWIDTH, &ImageWidth);
 	TIFFGetField( Tif, TIFFTAG_IMAGELENGTH, &ImageHeight);

int res = 0;

 double LowerRightX = ImageWidth;
 double LowerRightY = ImageHeight;

 res = GTIFImageToPCS(GTif, &LowerRightX, &LowerRightY);
  //Lower Left
 LowerLeftX = 0.0;
 LowerLeftY = ImageHeight;

 res = GTIFImageToPCS(GTif, &LowerLeftX, &LowerLeftY);

 //Upper Right
 UpperRightX = ImageWidth;
 UpperRightY = 0.0;

 res = GTIFImageToPCS(GTif, &UpperRightX, &UpperRightY);

 double UpperLeftX = 0.0;
 double UpperLeftY = 0.0;

 res = GTIFImageToPCS(GTif, &UpperLeftX, &UpperLeftY);

  	Ogre::ManualObject *Terrain = SceneManager->createManualObject("Terrain");
	Terrain->setDynamic(false);
	Terrain->begin("Terrain", Ogre::RenderOperation::OT_TRIANGLE_FAN);

#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))

#define SW     0
#define NW     1
#define NE     2
#define SE     3

FILE *File = NULL;
char            Dummy[160];
char inText[24];
 char *Dest;
	int             base[2048];         /* array of base elevations */
char            mapLabel[145];
int             DEMlevel, elevationPattern, groundSystem, groundZone;
double          projectParams[15];
int             planeUnitOfMeasure, elevUnitOfMeasure, polygonSizes;
double          groundCoords[4][2], elevBounds[2], localRotation;
int             accuracyCode;
double          spatialResolution[3];
int             profileDimension[2];
int             firstRow, lastRow;
int             wcount = 0;

double          verticalScale = 1.0;      /* to stretch or shrink elevations */
double          deltaY;
char *junk;

double eastMost;
double westMost;
double southMost;
double northMost;

int  eastMostSample;
int  westMostSample;
int southMostSample;
int northMostSample;

int        rowCount, columnCount, r, c;
int	rowStr = 1;
int rowEnd;
int colStr = 1;
int colEnd;
int colInt = 1;
int rowInt = 1;
int outType = 0;
 int k,l ;
 double comp = 0.0;

     int 	   mod ;
    int             tempInt, lastProfile = 0;

    float           noValue = 0.0f;
    int             profileID[2], profileSize[2];
    double          planCoords[2], localElevation, elevExtremea[2];


 File = fopen("C:\\Users\\Ilya\\Documents\\Visual Studio 2010\\Projects\\Recipes\\media\\geotiff\\karthaus_pa.dem", "r");

 fscanf(File, "%144c%6d%6d%6d%6d",
	     mapLabel,              /* 1 */
	     &DEMlevel,             /* 2 */
	     &elevationPattern,     /* 3 */
             &groundSystem,         /* 4 */
             &groundZone );         /* 5 */


    for (k=0; k<15 ; k++) 
	{
     fscanf(File,"%24c", inText);    /* 6 */
      Dest = strchr(inText,'D');  *Dest = 'E';
      projectParams[k]  = strtod(inText,&junk);
    }

    fscanf(File,"%6d%6d%6d",
             &planeUnitOfMeasure,   /* 7 */
             &elevUnitOfMeasure,    /* 8 */
             &polygonSizes);        /* 9 */

    for (k=0; k < 4 ; k++)
       for (l=0; l < 2 ; l++)
	   { 
        fscanf(File,"%24c", inText);    /* 6 */
        Dest = strchr(inText,'D');  *Dest = 'E';
        groundCoords[k][l]  = strtod(inText,&junk);
	   }

    fscanf(File,"%24c", inText);
    Dest = strchr(inText,'D');  *Dest = 'E';
    elevBounds[0] = strtod(inText,&junk);       /* 10 */
    fscanf(File,"%24c", inText);
    
	Dest = strchr(inText,'D');  *Dest = 'E';
    elevBounds[1] = strtod(inText,&junk);      /* 11 */
    fscanf(File,"%24c", inText);
    Dest = strchr(inText,'D');  *Dest = 'E';
    localRotation  = strtod(inText,&junk);      /* 12 */

   fscanf(File,"%1d%12le%12le%12le%6d%6d",
             &accuracyCode,             /* 13 */
	     &spatialResolution[0],     /* 14 */
             &spatialResolution[1],     /* 14 */
             &spatialResolution[2],     /* 14 */
             &profileDimension[0],      /* 15 */
             &profileDimension[1]);     /* 15 */

   fscanf(File, "%160c", Dummy);

    if (spatialResolution[0] == 3.0) 
	{
	 comp   = spatialResolution[0] - 1.0 ;
	 deltaY = spatialResolution[1] / 3600.  ;
    }
    else
	{
	 comp   = 0.;
	 deltaY = spatialResolution[1] ;
    }

    eastMost  = MAX(groundCoords[NE][0], groundCoords[SE][0]);
    westMost  = MIN(groundCoords[NW][0], groundCoords[SW][0]);
    northMost = MAX(groundCoords[NE][1], groundCoords[NW][1]);
    southMost = MIN(groundCoords[SW][1], groundCoords[SE][1]);
    eastMostSample =  ((int) (eastMost / spatialResolution[0]))
		     * (int) spatialResolution[0];
    westMostSample =  ((int) ((westMost + comp) / spatialResolution[0]))
		     * (int) spatialResolution[0] ;
    northMostSample = ((int) (northMost / spatialResolution[1]))
		     * (int) spatialResolution[1] ;
    southMostSample = ((int) ((southMost + comp) / spatialResolution[1]))
		     * (int) spatialResolution[1] ;
    
	columnCount = (eastMostSample  - westMostSample)
		  / (int) spatialResolution[0] + 1;
    rowCount    = (northMostSample - southMostSample)
		  / (int) spatialResolution[1] + 1;

    if (columnCount != profileDimension[1])
	  columnCount =MIN( profileDimension[1], columnCount);

for (c = 1; c <= columnCount; c++) 
 {
  fscanf(File, "%d%d%d%d",
		 &profileID[0],        /* 1 */
		 &profileID[1],        /* 1 */
		 &profileSize[0],      /* 2 */
		 &profileSize[1]);     /* 2 */

       fscanf(File,"%24c", inText);
       Dest = strchr(inText,'D');  *Dest = 'E';
       planCoords[0]   = strtod(inText,&junk);     /* 3 */
	  fscanf(File,"%24c", inText);
       Dest = strchr(inText,'D');  *Dest = 'E';
       planCoords[1]   = strtod(inText,&junk);     /* 3 */
	   fscanf(File,"%24c", inText);
       Dest = strchr(inText,'D');  *Dest = 'E';
       localElevation  = strtod(inText,&junk);     /* 4 */
	 fscanf(File,"%24c", inText);
       Dest = strchr(inText,'D');  *Dest = 'E';
       elevExtremea[0] = strtod(inText,&junk);     /* 5 */
	   fscanf(File,"%24c", inText);
       Dest = strchr(inText,'D');  *Dest = 'E';
       elevExtremea[1] = strtod(inText,&junk);     /* 5 */

       lastProfile =  profileID[1];

       firstRow = abs(((int) (planCoords[1] - southMostSample))
		 / (int) spatialResolution[1]);
       lastRow = firstRow + profileSize[0] - 1;

       for ( r = 0 ; r < firstRow  ; r++ ) 
		   base[r] = 0;

/*     read in all the data for this column */
       for (r = firstRow; r <= lastRow; r++ )
       {
	  fscanf(File, "%6d",	&tempInt);
	   base[r] = tempInt;
       }

    double          tempFloat;

   for (r = firstRow; r <= lastRow; r += rowInt) 
   {
    tempFloat = (float) base[r]  * verticalScale;
 	Terrain->position(planCoords[0], tempFloat, planCoords[1]);

	Ogre::Real u = 0.0;
	Ogre::Real v = 0.0;

	if (planCoords[0] > LowerLeftX && planCoords[0] < UpperRightX)
	{
		u = (planCoords[0] - LowerLeftX) / (UpperRightX - LowerLeftX);
	}

	if (planCoords[1] > LowerLeftY && planCoords[1] < UpperRightY)
	{
		v = (planCoords[1] - LowerLeftY) / (UpperRightY - LowerLeftY);
	}

	Terrain->textureCoord(u, v); 
	planCoords[1] += deltaY;
   }
 }

 Terrain->end();
 fclose(File);

	Ogre::SceneNode* node = SceneManager->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0, 0, 0);
	node->attachObject(Terrain);

	Ogre::AxisAlignedBox Box = Terrain->getBoundingBox();
	Ogre::Vector3 Center = Box.getCenter();

	Ogre::Vector3 Target = Center;
	Ogre::Vector3 Position = Center;

	Position[1] += 5000.0;
	m_Camera->setPosition(Position);
	m_Camera->lookAt(Target);
}


BOOL CGeoImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[2] = 0.1 * zDelta;

	CEngine * Engine = ((CGeoImageApp*)AfxGetApp())->m_Engine;
	if (Engine == NULL)
		return false;
	Ogre::Root *Root = Engine->GetRoot();
	if (m_Camera == NULL)
		return false;
	m_Camera->moveRelative(CameraMove);

	Root->renderOneFrame();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGeoImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseNavigation = false;

	CView::OnLButtonUp(nFlags, point);
}


void CGeoImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
		m_MouseNavigation = true;
			m_MousePosition = point;

	CView::OnLButtonDown(nFlags, point);
}


void CGeoImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseNavigation)
	{
	Ogre::Vector3 CameraMove(0.0, 0.0, 0.0);

	CameraMove[0] = -(m_MousePosition.x - point.x);
	CameraMove[1] = m_MousePosition.y - point.y;

	CEngine * Engine = ((CGeoImageApp*)AfxGetApp())->m_Engine;
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


void CGeoImageView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	Ogre::Root *Root = ((CGeoImageApp*)AfxGetApp())->m_Engine->GetRoot();
	Root->renderOneFrame();

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
