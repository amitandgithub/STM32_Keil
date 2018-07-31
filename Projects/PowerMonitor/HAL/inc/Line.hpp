/*
 * Line.hpp
 *
 *  Created on: 30-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_LINE_HPP_
#define APP_INC_LINE_HPP_

namespace App
{


class Line
{
public:

	const static unsigned char NO_OF_CHARS_IN_LINE = 12U;
	const static unsigned char NO_OF_HANDLERS_IN_LINE = 3U;
	typedef enum
	{
		Touch = 0,
		LongTouch,
		LongLongTouch,
		MaxEvents
	}Event_t;
	typedef void(*EventHandler_t)();
	Line();
	Line(const char* pTest,EventHandler_t Touch, EventHandler_t LongTouch, EventHandler_t LongLongTouch );
	~Line(){};
	inline void RegisterEventHandler(Event_t anEvent, EventHandler_t anEventHandler );
	inline char* GetName();
	void SetName(const char* pName);

	inline void HandleEvent(Event_t anEvent){ aEventHandlers[anEvent % MaxEvents](); }
	char Text[NO_OF_CHARS_IN_LINE];
	EventHandler_t aEventHandlers[MaxEvents];
private:



};






















}


















#endif /* APP_INC_LINE_HPP_ */
