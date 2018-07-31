/*
 * UI.cpp
 *
 *  Created on: 01-Nov-2017
 *      Author: Amit Chaudhary
 */

#include"UI.hpp"
#include"Line.hpp"
#include"Menu.hpp"
#include"Page.hpp"
#include"Nokia5110LCD.hpp"

using namespace std;

namespace App
{

unsigned char UI::ActiveScreen = 0; 
unsigned char UI::TotalRegisteredScreens = 0;
unsigned char UI::PreviousActiveScreen = 0;

UI::UI(Nokia5110LCD* m_pNokiaLCD): m_pNokiaLCD(m_pNokiaLCD)
{

}
bool UI::Init()
{
	m_pNokiaLCD->HwInit();
	m_pNokiaLCD->DrawLine(0, 0, "Initializing");
        TotalAddeedScreens = 0;
        for(uint8_t i = 0; i < NO_OF_SCREENS_IN_UI; i++)
        {
          m_aScreens[i] = nullptr;
        }
        
	return true;
}

void UI::EventHamdler(Screen::Event_t& rEvent)
{
	if( rEvent != Screen::MaxEvents )
	{
          m_aScreens[ActiveScreen % NO_OF_SCREENS_IN_UI]->EventHandler(rEvent);
	  rEvent = Screen::MaxEvents;
	}
}

void UI::DisplayScreen()
{ 
  if(m_aScreens[ActiveScreen % NO_OF_SCREENS_IN_UI]) 
  {
      m_pNokiaLCD->DrawBuffer( m_aScreens[ActiveScreen % NO_OF_SCREENS_IN_UI]->GetScreenTextArray() );
  }
  else
  {
    if(m_aScreens[0])
      m_pNokiaLCD->DrawBuffer( m_aScreens[0]->GetScreenTextArray() );
  }
}














}


