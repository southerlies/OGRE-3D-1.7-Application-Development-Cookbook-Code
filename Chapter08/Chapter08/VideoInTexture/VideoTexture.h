#pragma once
#include <Vfw.h>
#include <Ogre.h>

class CVideoTexture
{
public:
	CVideoTexture(void);
	~CVideoTexture(void);
	CVideoTexture(Ogre::SceneManager *SceneManager, Ogre::Real Width, Ogre::Real Height, LPCTSTR szFile);

	Ogre::SceneManager *m_SceneManager;

	int m_FrameNumber;
	
	PGETFRAME  m_Frame;
	BITMAPFILEHEADER m_BMPFileHeader;
	PAVIFILE   m_aviFile;
	PAVISTREAM  m_aviStream;
	AVISTREAMINFO  m_aviStreamInfo;
	BITMAPINFOHEADER m_bmpInfo;

	Ogre::HardwarePixelBufferSharedPtr m_PixelBuffer;
};

