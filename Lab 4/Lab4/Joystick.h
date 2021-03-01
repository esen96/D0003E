#include <stdbool.h>
#include "PulseGenerator.h"
#include "TinyTimber.h"

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef struct {
	Object super;
	PulseGenerator *pulseGenerator1;
	PulseGenerator *pulseGenerator2;
	PulseGenerator *currentGenerator;
	bool risingEdge;
} Joystick;

void switchGenerator(Joystick *self, int arg);
void changeFrequency(Joystick *self, int arg);
void hold(Joystick *self, int arg);

#define initJoystick(pg1, pg2) { initObject(), pg1, pg2, pg1, false }

#endif /* JOYSTICK_H_ */