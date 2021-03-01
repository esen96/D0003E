#include <avr/io.h>
#include "initAVR.h"
#include "TinyTimber.h"
#include "Joystick.h"
#include "InterruptHandler.h"
#include "PulseGenerator.h"
#include "LCD.h"
#include "PortWriter.h"


int main(void) {
    
	initAVR();
	PortWriter pw = initPortWriter();
	PulseGenerator pg1 = initPulseGenerator(4, 4, &pw);
	PulseGenerator pg2 = initPulseGenerator(0, 6, &pw);
	printAt(pg1.frequency, pg1.pos);
	printAt(pg2.frequency, pg2.pos);
	Joystick js = initJoystick(&pg1, &pg2);
	InterruptHandler interruptHandler = initInterruptHandler(&js);
	INSTALL(&interruptHandler, Switch, IRQ_PCINT0);
	INSTALL(&interruptHandler, Change, IRQ_PCINT1);
	return TINYTIMBER(&pg1, output, NULL);
}
