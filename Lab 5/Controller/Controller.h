#include <avr/io.h>
#include <stdbool.h>
#include "TinyTimber.h"
#include "Bridge.h"

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

typedef struct {
	Object super;
	int northQueue;
	int southQueue;
	bool alternate;
	int carsPassed;
	LCD *lcd;
	Bridge *bridge;
} Controller;

void northboundArrival(Controller *self, int arg);
void southboundArrival(Controller *self, int arg);
void northboundEntry(Controller *self, int arg);
void southboundEntry(Controller *self, int arg);
void dispatch(Controller *self, int arg);
void transmit(Controller *self, uint8_t sig);
void alternateCarStreams(Controller *self, int arg);

#define initController(myLCD, myBridge) { initObject(), 0, 0, true, 0, myLCD, myBridge }

#endif /* CONTROLLER_H_ */