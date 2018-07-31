/*
 * Line.cpp
 *
 *  Created on: 30-Oct-2017
 *      Author: Amit Chaudhary
 */

#include"Line.hpp"

namespace App
{

Line::Line(const char* pTest,EventHandler_t Touch, EventHandler_t LongTouch, EventHandler_t LongLongTouch )
{

	for(int i = 0; i < NO_OF_CHARS_IN_LINE; i++)
	{
		Line::Text[i] = ' ';
	}

	for(int i = 0; i < NO_OF_CHARS_IN_LINE &&  pTest[i] ; i++)
	{
		Line::Text[i] = pTest[i];
	}
	aEventHandlers[0] = Touch;
	aEventHandlers[1] = LongTouch;
	aEventHandlers[2] = LongLongTouch;
}

Line::Line()
{

	for(int i = 0; i < NO_OF_CHARS_IN_LINE; i++)
	{
		Line::Text[i] = '.';
	}

	for(int i = 0; i < MaxEvents; i++)
	{
		aEventHandlers[i] = nullptr;
	}

}
inline void Line::RegisterEventHandler(Event_t anEvent, EventHandler_t anEventHandler )
{
	aEventHandlers[anEvent] = anEventHandler;
}

inline char* Line::GetName()
{
	return Text;
}

void Line::SetName(const char* pName)
{

	for(int i = 0; i < NO_OF_CHARS_IN_LINE; i++)
	{
		Text[i] = pName[i];
	}
}


















}


