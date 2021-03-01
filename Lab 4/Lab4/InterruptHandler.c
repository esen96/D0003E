#include <avr/io.h>
#include "InterruptHandler.h"
#include "Joystick.h"
#include "TinyTimber.h"

/* Interrupt function for switching the active pulse generator */
void Switch(InterruptHandler *self, int arg) {
	ASYNC(self->joystick, switchGenerator, 0);
}

/* Interrupt function for changing the active pulse generator frequency */
void Change(InterruptHandler *self, int arg) {
	ASYNC(self->joystick, changeFrequency, 0);
}