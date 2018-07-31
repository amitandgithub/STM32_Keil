/*
 * UI.hpp
 *
 *  Created on: 01-Nov-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_UI_HPP_
#define APP_INC_UI_HPP_

#include"Line.hpp"
#include"Menu.hpp"
#include"Nokia5110LCD.hpp"
#include"Page.hpp"
#include <cstring>
#include <Screen.hpp>

namespace App
{


class UI
{
public:
	const static unsigned char NO_OF_MENUS_IN_UI = 10U;
        constexpr static unsigned char NO_OF_SCREENS_IN_UI = 10U;
        typedef uint8_t ScreenHandle_t;

	UI(Nokia5110LCD* m_pNokiaLCD);
	~UI(){};
	bool Init();        
        inline ScreenHandle_t AddScreen(Screen* pScreen ){m_aScreens[TotalAddeedScreens%NO_OF_SCREENS_IN_UI] = pScreen; return TotalAddeedScreens++;}// ++TotalRegisteredScreens %= NO_OF_SCREENS_IN_UI ;}
        inline void AddScreen(Screen* pScreen, unsigned char Slot ){m_aScreens[Slot%NO_OF_SCREENS_IN_UI] = pScreen;}// ++TotalRegisteredScreens %= NO_OF_SCREENS_IN_UI;}
        inline static void GoToPreviousScreen(){SetActiveScreen(PreviousActiveScreen);  }
        inline static void GoToNextScreen(){ PreviousActiveScreen = ActiveScreen; ++ActiveScreen %= TotalRegisteredScreens ; }
        inline static ScreenHandle_t GetActiveScreen(){return ActiveScreen; }
        inline static void SetActiveScreen(ScreenHandle_t ScreenNo){ PreviousActiveScreen = ActiveScreen; ActiveScreen = ScreenNo; }
        void DisplayScreen();//{ if(m_aScreens[ActiveScreen % NO_OF_SCREENS_IN_UI]) m_pNokiaLCD->DrawBuffer( m_aScreens[ActiveScreen % NO_OF_SCREENS_IN_UI]->GetScreenTextArray() );}
       	inline void Run(){DisplayScreen();}
        void EventHamdler(Screen::Event_t& rEvent);

private:
        Screen* m_aScreens[NO_OF_SCREENS_IN_UI];
        static ScreenHandle_t ActiveScreen;
        static unsigned char PreviousActiveScreen;
        static unsigned char TotalRegisteredScreens;
        unsigned char TotalAddeedScreens;
	Nokia5110LCD* m_pNokiaLCD;
};

























}

























#endif /* APP_INC_UI_HPP_ */
