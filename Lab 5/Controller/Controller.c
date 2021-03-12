#include <avr/io.h>
#include "Controller.h"
#include "LCD.h"

#define REDSIGNAL 0xA
#define SOUTHGREENSIGNAL 0x6
#define NORTHGREENSIGNAL 0x9
#define DISPATCHPOLLRATE 2
#define CARSTREAMRATE 1
#define MAXSTREAMSIZE 4
#define TRAVELTIME 5


/* Handle northbound car arrival sensor activation */
void northboundArrival(Controller *self, int arg) {
	self->northQueue++;
	ASYNC(self->lcd, updateNorth, self->northQueue);

}

/* Handle southbound car arrival sensor activation */
void southboundArrival(Controller *self, int arg) {
	self->southQueue++;
	ASYNC(self->lcd, updateSouth, self->southQueue);
}

/* Handle northbound bridge entry sensor activation */
void northboundEntry(Controller *self, int arg) {
	self->northQueue--;
	ASYNC(self->lcd, updateNorth, self->northQueue);
	ASYNC(self->bridge, addNorthboundCar, 0);
}

/* Handle southbound bridge entry sensor activation */
void southboundEntry(Controller *self, int arg) {
	self->southQueue--;
	ASYNC(self->lcd, updateSouth, self->southQueue);
	ASYNC(self->bridge, addSouthboundCar, 0);
}

/* Dispatches traffic light signals to the simulator */
void dispatch(Controller *self, int arg) {
	if (self->northQueue == 0 && self->southQueue == 0) {
		ASYNC(self, transmit, REDSIGNAL);
		AFTER(SEC(DISPATCHPOLLRATE), self, dispatch, 0);
	}
	else if (self->northQueue > 0 && self->southQueue == 0) {
		if (self->bridge->southBoundCars > 0) {		// Let southbound cars pass
			AFTER(SEC(TRAVELTIME), self, dispatch, 0);
		}
		else {
			ASYNC(self, transmit, NORTHGREENSIGNAL);
			AFTER(SEC(CARSTREAMRATE), self, dispatch, 0);
		}
	}
	else if (self->southQueue > 0 && self->northQueue == 0) {
		if (self->bridge->northBoundCars > 0) {		// Let northbound cars pass
			AFTER(SEC(TRAVELTIME), self, dispatch, 0);
		}
		else {
			ASYNC(self, transmit, SOUTHGREENSIGNAL);
			AFTER(SEC(CARSTREAMRATE), self, dispatch, 0);
		}
	}
	else {
		ASYNC(self, alternateCarStreams, 0);
	}
}

/* Lets a stream of cars pass from alternate sides */
void alternateCarStreams(Controller *self, int arg) {
	if (self->alternate == true) {
		if (self->carsPassed < MAXSTREAMSIZE && self->northQueue > 0) {
			ASYNC(self, transmit, NORTHGREENSIGNAL);
			self->carsPassed++;
			AFTER(SEC(CARSTREAMRATE), self, dispatch, 0);
		} else {
			self->alternate = false;
			self->carsPassed = 0;
			ASYNC(self, transmit, REDSIGNAL);
			AFTER(SEC(TRAVELTIME), self, dispatch, 0);
		}
	}
	else {
		if (self->carsPassed < MAXSTREAMSIZE && self->southQueue > 0) {
			ASYNC(self, transmit, SOUTHGREENSIGNAL);
			self->carsPassed++;
			AFTER(SEC(CARSTREAMRATE), self, dispatch, 0);
		} else {
			self->alternate = true;
			self->carsPassed = 0;
			ASYNC(self, transmit, REDSIGNAL);
			AFTER(SEC(TRAVELTIME), self, dispatch, 0);
		}
	}
}

/* Transmit traffic light information to the USART data register */
void transmit(Controller *self, uint8_t sig) {
	
	/* Wait for empty transmit buffer */ 
	while ( !(UCSR0A & ( 1<< UDRE0)) );  
	
	/* Transmit signal */
	UDR0 = sig;
}