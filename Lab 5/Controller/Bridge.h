#include <stdbool.h>
#include "TinyTimber.h"
#include "LCD.h"


#ifndef BRIDGE_H_
#define BRIDGE_H_

typedef struct {
	Object super;
	int southBoundCars;
	int northBoundCars;
	LCD *lcd;
} Bridge;

void addNorthboundCar(Bridge *self, int arg);
void addSouthboundCar(Bridge *self, int arg);
void removeNorthboundCar(Bridge *self, int arg);
void removeSouthboundCar(Bridge *self, int arg);

#define initBridge(myLCD) { initObject(), 0, 0, myLCD }

#endif /* BRIDGE_H_ */