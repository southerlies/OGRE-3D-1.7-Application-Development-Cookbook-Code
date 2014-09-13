#pragma once
#include "stdafx.h"
#include <sphelper.h>
#include "Ogre.h"
using namespace Ogre;

class CSoundController :	public ControllerValue<Real>
{
public:

CSoundController(CString SoundPath, CComPtr<ISpVoice> Voice)
{
	m_SoundPath = SoundPath;
	m_Voice = Voice;
	m_SoundIndex = 0;

	Sounds[0] = "boatsteam.wav";
	Sounds[1] = "carcrash.wav";
	Sounds[2] = "earlycarengine.wav";
	Sounds[3] = "ferrari250.wav";
	Sounds[4] = "ghostbustersray.wav";
	Sounds[5] = "goodmorningvietnam.wav";
	Sounds[6] = "harley.wav";
	Sounds[7] = "harrypotter.wav";
	Sounds[8] = "hondamotorcycle.wav";
	Sounds[9] = "jetplanef16.wav";
}

Real getValue(void) const
{
	return m_SoundIndex;
}

void setValue(Real value)
{
	CComPtr<ISpStream> cpWavStream;

	int Index = (int)(value * 10);
	
	if ((int)m_SoundIndex == Index)
	{
//		m_Voice->Pause();
	}
	else
	{
		m_SoundIndex = Index;

		CString SoundPath = m_SoundPath + "\\" + Sounds[Index];
		SPBindToFile(SoundPath, SPFM_OPEN_READONLY, &cpWavStream);
		m_Voice->SpeakStream(cpWavStream, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
//		m_Voice->Resume();
	}
}

	CString Sounds[10];

	CString m_SoundPath;
	Real m_SoundIndex;
	CComPtr<ISpVoice> m_Voice;
};

