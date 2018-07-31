

#ifndef APP_INC_LED_HPP_
#define APP_INC_LED_HPP_

#include "GpioOutput.hpp"

//using namespace Bsp;

class Led : public Bsp::GpioOutput
{

public:
	enum{OFF=0,ON};
	Led(PORT Port, u16 Pin);
	bool Get_State(){return Led_State; }
	void Blink(unsigned int delay = 0xFFFFF);
	void Blink(unsigned int on,unsigned int off);
    void MultiBlink(unsigned int Blinks);
	void On(void);
	void Off(void);
	inline void Toggle(void){ToggleOutput();}
private:
	bool Led_State;
	void Set_State(bool state){Led_State = state;}
};

#endif