/*
 * Page.hpp
 *
 *  Created on: 24-Dec-2017
 *      Author: Amit Chaudhary
 */

#ifndef HAL_INC_PAGE_HPP_
#define HAL_INC_PAGE_HPP_

#include <cstring>

namespace App
{

class Page
{
public:
	const static unsigned char NO_OF_CHARS_IN_LINE = 12U;
	//const static unsigned char NO_OF_HANDLERS_IN_LINE = 3U;
	const static unsigned char NO_OF_LINES_IN_PAGE = 6U;
	//const static char m_Pointer = '>';
	//const static char m_Space = ' ';
	//constexpr static unsigned char LAST_LINE = (NO_OF_LINES_IN_MENU-1U);
	//const static unsigned char FIRST_LINE = 0U;
	constexpr static unsigned char SIZE_OF_PAGE = ( NO_OF_CHARS_IN_LINE * NO_OF_LINES_IN_PAGE) ;

	Page();

	~Page(){};

	//bool AddLine(Line &rLine, unsigned char LineNumber);

	//void DeleteLine(unsigned char LineNumber){};

	//void EventHandler(Event_t anEvent);

	//inline unsigned char GetActiveLine() const {return ActiveLine;}

	void SetPageText(unsigned char Line, unsigned char Col, const char* pText, unsigned char Len);
        
        void SetPageText(const char* pText);
private:
	char PageText[NO_OF_LINES_IN_PAGE * NO_OF_CHARS_IN_LINE];


};


}
#endif 