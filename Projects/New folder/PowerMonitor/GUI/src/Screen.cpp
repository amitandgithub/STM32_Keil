/*
 * Screen.cpp
 *
 *  Created on: 25-Dec-2017
 *      Author: Amit Chaudhary
 */

#include <Screen.hpp>

namespace Bsp {

Screen::Screen()
{
	 std::memset(&ScreenText[0],'.',SIZE_OF_SCREEN);
}

Screen::~Screen()
{

}

void Screen::AddText(char* pText)
{
  std::memcpy(ScreenText, pText, SIZE_OF_SCREEN);
}

void Screen::SetText(unsigned char Line, unsigned char Col, const char* pText, unsigned char Len)
{
    Line %= NO_OF_LINES_IN_SCREEN;
    Col  %= NO_OF_CHARS_IN_LINE;

    for(uint8_t i = 0; i< Len; i++)
    {
        ScreenText[(Line*NO_OF_CHARS_IN_LINE)+Col+i] = pText[i];
    }
  //std::memcpy(&ScreenText[ ((Line % NO_OF_LINES_IN_SCREEN) * NO_OF_CHARS_IN_LINE) + (Col%NO_OF_CHARS_IN_LINE) ],pText, Len%((unsigned char)(NO_OF_CHARS_IN_LINE-Col)) );
  //                       (Lines 0 - 5)                                          ( Col 0 - 13)                     ( Len 0-13 )
}

void Screen::SetText(const char* pText)
{
  if(pText)
  {
      std::memcpy(&ScreenText[0],pText, SIZE_OF_SCREEN );
  }
}
void Screen::EventHandler(Event_t anEvent)
{
  //unsigned char LActiveScreen = App::UI::GetActiveScreen();

    if(anEvent == Touch)
	{
           App::UI::GoToPreviousScreen();
	}
	else if(anEvent == LongTouch)
	{
            App::UI::GoToNextScreen();
	}
	else if(anEvent == LongLongTouch)
	{
            //App::UI::GoToNextScreen();
	}
	else
	{
		// Nothing To Do
	}

}


} /* namespace Bsp */