#pragma once

#include "Ogre.h"
using namespace Ogre;

class LightControl : public ControllerValue<Real>
{
 protected:
     
	 Light* m_Light;
     Billboard* m_Billboard;
     ColourValue m_MaxColor;
     Real m_Intensity;

 public:

     LightControl(Light* Light, Billboard* Billboard, ColourValue maxColor)
     {
         m_Light = Light;
         m_Billboard = Billboard;
         m_MaxColor = maxColor;
		 m_Intensity = 1.0;
     }
 
     virtual Real  getValue(void) const
     {
         return m_Intensity;
     }
 
     virtual void  setValue (Real Value)
     {
         m_Intensity = Value; 
         ColourValue newColor;
 
         newColor.r = m_MaxColor.r * m_Intensity;
         newColor.g = m_MaxColor.g * m_Intensity;
         newColor.b = m_MaxColor.b * m_Intensity; 
 
	     m_Light->setDiffuseColour(newColor);
         m_Billboard->setColour(newColor);
     }
 };

