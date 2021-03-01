#include "TinyTimber.h"
#include "Joystick.h"

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_


typedef struct {
	Object super;
	Joystick *joystick;
} InterruptHandler;

void Switch(InterruptHandler *self, int arg);
void Change(InterruptHandler *self, int arg);

#define initInterruptHandler(js) { initObject(), js }


#endif /* INTERRUPTHANDLER_H_ */