#include "StdAfx.h"
#include "VideoTexture.h"
#include <Vfw.h>
#include <Ogre.h>


CVideoTexture::CVideoTexture(Ogre::SceneManager *SceneManager, Ogre::Real Width, Ogre::Real Height, LPCTSTR szFile)
{
	Ogre::TexturePtr VideoTexture = Ogre::TextureManager::getSingleton().createManual(
		"Video", 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D, 640, 480, 0, Ogre::PF_R8G8B8A8, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

	m_PixelBuffer = VideoTexture->getBuffer();

	Ogre::ManualObject *Screen = SceneManager->createManualObject("Screen");
	Screen->setDynamic(true);
	Screen->begin("Video", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(Width,0,0);
	Screen->textureCoord(1,0);

	Screen->position(Width,Height,0);
	Screen->textureCoord(1,1);

	Screen->triangle(0, 1, 2);

	Screen->position(0,0,0);
	Screen->textureCoord(0,0);

	Screen->position(Width,Height,0);
	Screen->textureCoord(1,1);

	Screen->position(0,Height,0);
	Screen->textureCoord(0,1);

	Screen->triangle(3, 4, 5);
	 
	Screen->end();

	Ogre::SceneNode* node = SceneManager->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0, 0, 0);
	node->attachObject(Screen);
		
	AVIFileInit();
	AVIFileOpen(&m_aviFile, szFile, OF_READ,NULL);

	AVIFileGetStream(m_aviFile, &m_aviStream, streamtypeVIDEO, 0);
	AVIFileRelease(m_aviFile);
	AVIStreamInfo(m_aviStream, &m_aviStreamInfo, sizeof(m_aviStreamInfo));

	memset(&m_bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	m_bmpInfo.biSize = sizeof(BITMAPINFOHEADER) ; 
	m_bmpInfo.biBitCount = 32; 
	m_bmpInfo.biCompression = BI_RGB;
	m_bmpInfo.biHeight = 480;
	m_bmpInfo.biWidth = 640;
	m_bmpInfo.biPlanes = 1;
	m_bmpInfo.biSizeImage = 0;
      
	m_FrameNumber = 0;
	m_Frame = AVIStreamGetFrameOpen(m_aviStream, &m_bmpInfo); 
}


CVideoTexture::~CVideoTexture(void)
{
}

