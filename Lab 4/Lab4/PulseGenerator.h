#include "TinyTimber.h"
#include "PortWriter.h"

#ifndef PULSEGENERATOR_H_
#define PULSEGENERATOR_H_

typedef struct {
	Object super;
	int frequency;
	int storedFrequency;
	int pos;
	int portBit;
	PortWriter *portWriter;
} PulseGenerator;

void increment(PulseGenerator *self, int arg);
void decrement(PulseGenerator *self, int arg);
void reset(PulseGenerator *self, int arg);
void restore(PulseGenerator *self, int arg);
void output(PulseGenerator *self, int arg);
void shutDown(PulseGenerator *self, int arg);

#define initPulseGenerator(lcdPos, pbit, pw) { initObject(), 0, 0, lcdPos, pbit, pw }

#endif /* PULSEGENERATOR_H_ */