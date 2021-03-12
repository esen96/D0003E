#include <avr/io.h>
#include "TinyTimber.h"
#include "initAVR.h"
#include "InterruptHandler.h"
#include "Controller.h"
#include "Bridge.h"
#include "LCD.h"


int main(void)
{
    initAVR();
	LCD lcd = initLCD();
	Bridge bridge = initBridge(&lcd);
	Controller controller = initController(&lcd, &bridge);
	InterruptHandler interruptHandler = initInterruptHandler(&controller);
	INSTALL(&interruptHandler, receive, IRQ_USART0_RX);
	return TINYTIMBER(&controller, dispatch, 0);
}

