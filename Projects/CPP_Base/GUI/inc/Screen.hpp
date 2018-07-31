/*
 * Screen.hpp
 *
 *  Created on: 25-Dec-2017
 *      Author: Amit Chaudhary
 */

#include <cstring>
#include<stdio.h>
#include"UI.hpp"

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

namespace Bsp
{

class Screen
{
public:
  	typedef void(*EventHandler_t)();

	typedef enum
	{
		Touch = 0,
		LongTouch,
		LongLongTouch,
		MaxEvents
	}Event_t;

	Screen();
	virtual ~Screen();
    const static unsigned char NO_OF_CHARS_IN_LINE   = 14U;
	const static unsigned char NO_OF_LINES_IN_SCREEN = 6U;
	constexpr static unsigned char SIZE_OF_SCREEN = ( NO_OF_CHARS_IN_LINE * NO_OF_LINES_IN_SCREEN) ;

    void AddText(char* pText);

    virtual void EventHandler(Event_t anEvent);

    virtual inline char * GetScreenTextArray(){return ScreenText;}

	virtual void SetText(unsigned char Line, unsigned char Col, const char* pText, unsigned char Len);

    void SetText(const char* pText);
private:
	char ScreenText[NO_OF_LINES_IN_SCREEN * NO_OF_CHARS_IN_LINE];
};

} /* namespace Bsp */

#endif /* SCREEN_HPP_ */