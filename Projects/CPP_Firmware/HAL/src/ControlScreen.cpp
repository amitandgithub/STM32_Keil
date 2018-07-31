/*
 * ControlScreen.cpp
 *
 *  Created on: 25-Dec-2017
 *      Author: Amit Chaudhary
 */

#include <ControlScreen.hpp>

namespace Bsp {

ControlScreen::ControlScreen(): ActiveLine(0)
{
  m_Pointer = '>';
  m_Space = ' ';

  for(int i = 0; i<Screen::NO_OF_LINES_IN_SCREEN * ControlScreen::NO_OF_HANDLERS_IN_LINE; i++)
		m_EventHandlers[i] = nullptr;
}

ControlScreen::~ControlScreen()
{

}

void ControlScreen::AddHandler(unsigned char LineNo, EventHandler_t LongTouchHandler, EventHandler_t LLongTouchHandler)
{
   m_EventHandlers[(LineNo % NO_OF_LINES_IN_SCREEN)*NO_OF_HANDLERS_IN_LINE + 0 ] =  LongTouchHandler;
   m_EventHandlers[(LineNo % NO_OF_LINES_IN_SCREEN)*NO_OF_HANDLERS_IN_LINE + 1 ] =  LLongTouchHandler;
}

void ControlScreen::SetText(unsigned char Line, unsigned char Col, const char* pText, unsigned char Len)
{
    Line %= NO_OF_LINES_IN_SCREEN;
    Col  %= NO_OF_CHARS_IN_LINE;

    for(uint8_t i = 0; i< Len; i++)
    {
        (m_Screen.GetScreenTextArray())[(Line*NO_OF_CHARS_IN_LINE)+Col+i] = pText[i];
    }
}
void ControlScreen::EventHandler(Event_t anEvent)
{
	if(anEvent == Touch)
	{
		//if(EventHandlers[ActiveLine*Menu::NO_OF_HANDLERS_IN_LINE + Touch ])
			//EventHandlers[ActiveLine*Menu::NO_OF_HANDLERS_IN_LINE + Touch ]();

		MovePointer(Down);
	}
	else if(anEvent == LongTouch)
	{
		if(m_EventHandlers[ActiveLine*ControlScreen::NO_OF_HANDLERS_IN_LINE + 0 ])
			m_EventHandlers[ActiveLine*ControlScreen::NO_OF_HANDLERS_IN_LINE + 0 ]();
	}
	else if(anEvent == LongLongTouch)
	{
		if(m_EventHandlers[ActiveLine*ControlScreen::NO_OF_HANDLERS_IN_LINE + 1 ])
			m_EventHandlers[ActiveLine*ControlScreen::NO_OF_HANDLERS_IN_LINE + 1 ]();
	}
	else
	{
		// Nothing To Do
	}
}

void ControlScreen::MovePointer(Direction_t Direction)
{
	//clear the pointer from current line
	//SetLineChar(static_cast<unsigned char>(ActiveLine), 0, m_Space );
        m_Screen.SetText(ActiveLine, 0, (const char*)&m_Space, 1);

	if( (ActiveLine == FIRST_LINE) && (Direction == Up) )
	{
		// Roll over to the last line
		ActiveLine = LAST_LINE;
	}
	else if( (ActiveLine == LAST_LINE) && (Direction == Down) )
	{
		// Roll over to the last line
		ActiveLine = FIRST_LINE;
	}
	else if( Direction == Down )
	{
		// Roll over to the last line
		ActiveLine += 1;
	}
	else if( Direction == Up )
	{
		// Roll over to the First line
		ActiveLine -= 1;

	}
	else
	{

	}
	//SetLineChar(static_cast<unsigned char>(ActiveLine), 0, m_Pointer );
        m_Screen.SetText(ActiveLine, 0, (const char*)&m_Pointer, 1);
}

} /* namespace Bsp */