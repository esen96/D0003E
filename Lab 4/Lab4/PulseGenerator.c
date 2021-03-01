#include <avr/io.h>
#include "PulseGenerator.h"
#include "TinyTimber.h"

#define PORTBIT4 0x10
#define PORTBIT6 0x40
#define HALFSEC 500

/* Increments the frequency of the pulse generator */
void increment(PulseGenerator *self, int arg) {
	if (self->frequency==0) {
		self->frequency++;
		AFTER((MSEC(HALFSEC)), self, output, NULL);
		} else if (self->frequency<99) {
		self->frequency++;
	}
}

/* Decrements the frequency of the pulse generator */
void decrement(PulseGenerator *self, int arg) {
	if (self->frequency>0) {
		self->frequency--;
	}
}

/* Stores the frequency of the active pulse generator and resets it to zero */
void reset(PulseGenerator *self, int arg) {
	self->storedFrequency = self->frequency;
	self->frequency=0;
	
}

/* Restores the frequency of the pulse generator to its stored value */
void restore(PulseGenerator *self, int arg) {
	self->frequency = self->storedFrequency;
	ASYNC(self, output, 0);
}

/* Outputs the frequency to port E */
void output(PulseGenerator *self, int arg) {
	if (self->frequency>0) {
		ASYNC(self->portWriter, write, self->portBit);
		AFTER((MSEC(HALFSEC)/self->frequency), self, output, 0);
		} else {
		ASYNC(self->portWriter, deactivate, self->portBit);
	}
}