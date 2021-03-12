#include "TinyTimber.h"
#include "Controller.h"

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_


typedef struct {
	Object super;
	Controller *controller;
} InterruptHandler;

void receive(InterruptHandler *self, int arg);

#define initInterruptHandler(myController) { initObject(), myController }


#endif /* INTERRUPTHANDLER_H_ */