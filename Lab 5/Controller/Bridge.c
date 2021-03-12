#include <avr/io.h>
#include "Bridge.h"

#define TRAVELTIME 5

/* Adds a northbound car to the bridge */
void addNorthboundCar(Bridge *self, int arg) {
	self->northBoundCars++;
	ASYNC(self->lcd, updateBridge, self->northBoundCars);
	AFTER(SEC(TRAVELTIME), self, removeNorthboundCar, 0);
}

/* Adds a southbound car to the bridge */
void addSouthboundCar(Bridge *self, int arg) {
	self->southBoundCars++;
	ASYNC(self->lcd, updateBridge, self->southBoundCars);
	AFTER(SEC(TRAVELTIME), self, removeSouthboundCar, 0);
}

/* Removes a northbound car from the bridge */
void removeNorthboundCar(Bridge *self, int arg) {
	if (self->northBoundCars > 0) {
		self->northBoundCars--;
		ASYNC(self->lcd, updateBridge, self->northBoundCars);
	} 
}

/* Removes a southbound car from the bridge */
void removeSouthboundCar(Bridge *self, int arg) {
	if (self->southBoundCars > 0) {
		self->southBoundCars--;
		ASYNC(self->lcd, updateBridge, self->southBoundCars);
	}
}