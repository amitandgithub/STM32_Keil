/*
 * Menu.hpp
 *
 *  Created on: 30-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_MENU_HPP_
#define APP_INC_MENU_HPP_

#include"Line.hpp"
#include"Nokia5110LCD.hpp"
#include <cstring>

namespace App
{

class Menu
{
public:
	typedef void(*EventHandler_t)();
	typedef enum
	{
		Up = 0,
		Down

	}Direction_t;
	typedef enum
	{
		Touch = 0,
		LongTouch,
		LongLongTouch,
		MaxEvents
	}Event_t;

	const static signed char NO_OF_CHARS_IN_LINE = 12U;
	const static signed char NO_OF_HANDLERS_IN_LINE = 3U;
	const static signed char NO_OF_LINES_IN_MENU = 6U;
	const static char m_Pointer = '>';
	const static char m_Space = ' ';
	constexpr static signed char LAST_LINE = (NO_OF_LINES_IN_MENU-1U);
	const static signed char FIRST_LINE = 0U;
	constexpr static signed char SIZE_OF_MENU = ( (NO_OF_LINES_IN_MENU * NO_OF_CHARS_IN_LINE) + (NO_OF_LINES_IN_MENU * NO_OF_HANDLERS_IN_LINE * sizeof(EventHandler_t)) ) ;

	//Menu(Nokia5110LCD* m_pNokiaLCD);

	Menu();
        Menu(EventHandler_t* pEventHandlers);

	~Menu(){};

	bool AddLine(Line &rLine, unsigned char LineNumber);

	void DeleteLine(unsigned char LineNumber){};

	void EventHandler(Event_t anEvent);

	inline unsigned char GetActiveLine() const {return ActiveLine;}

	void SetLineText(signed char Line,signed char Col, const char* pName);

	void SetLineChar(signed char Line,signed char Col, char Ch);
  
        inline char * GetMenuAddress(){return MenuText;}
        
        EventHandler_t* GetEventHandlerInstance(){return m_pEventHandlers; }


	//EventHandler_t  __attribute__ ((aligned (2))) EventHandlers[NO_OF_LINES_IN_MENU * NO_OF_HANDLERS_IN_LINE];

private:
  	char MenuText[NO_OF_LINES_IN_MENU * NO_OF_CHARS_IN_LINE];

	EventHandler_t* m_pEventHandlers;//[NO_OF_LINES_IN_MENU * NO_OF_HANDLERS_IN_LINE];
	void MovePointer(Direction_t Direction);
	char ActiveLine;
	//Nokia5110LCD* m_pNokiaLCD;

};






















}




































#endif /* APP_INC_MENU_HPP_ */
