#include <avr/io.h>
#include <stdbool.h>
#include "TinyTimber.h"
#include "LCD.h"
#include "Joystick.h"


#define LEFT 2
#define RIGHT 3
#define PRESS 4
#define UP 6
#define DOWN 7

#define INERTIA 300
#define HOLDSPEED 100
#define MAXCLICKSPEED 100



/* Switches current pulse generator */
void switchGenerator(Joystick *self, int arg) {
	if (!(PINE>>LEFT & 1)) {
		self->currentGenerator = self->pulseGenerator2;
		LCDDR3=0x0;
		LCDDR13=0x1;
	} else if (!(PINE>>RIGHT & 1)) {
		self->currentGenerator = self->pulseGenerator1;
		LCDDR3=0x1;
		LCDDR13=0x0;
	}
}


/* Changes frequency of the current pulse generator */
void changeFrequency(Joystick *self, int arg) {
	if (!(PINB>>PRESS & 1)) {
		if(self->currentGenerator->frequency == 0) {
			ASYNC(self->currentGenerator, restore, 0);
		} else {
			ASYNC(self->currentGenerator, reset, 0);
		}
	} else if (!(PINB>>UP & 1) && !self->risingEdge) {
		BEFORE(MSEC(MAXCLICKSPEED), self->currentGenerator, increment, 0); 
		hold(self, 0);
	} else if (!(PINB>>DOWN & 1) && !self->risingEdge) {
		BEFORE(MSEC(MAXCLICKSPEED), self->currentGenerator, decrement, 0);
		hold(self, 0);
	}
	printAt(self->currentGenerator->frequency, self->currentGenerator->pos);
}

/* Enables continuous increments or decrements when the joystick is held up or down */
void hold(Joystick *self, int arg) {
	if (!self->risingEdge) {
		self->risingEdge = true;
		AFTER(MSEC(INERTIA), self, hold, 0);
	} else if (!(PINB>>UP & 1)) {
		ASYNC(self->currentGenerator, increment, 0);
		printAt(self->currentGenerator->frequency, self->currentGenerator->pos);
		AFTER(MSEC(HOLDSPEED), self, hold, 0);
	} else if (!(PINB>>DOWN & 1)) {
		ASYNC(self->currentGenerator, decrement, 0);
		printAt(self->currentGenerator->frequency, self->currentGenerator->pos);
		AFTER(MSEC(HOLDSPEED), self, hold, 0);
	} else {
		self->risingEdge = false;
	}
}

