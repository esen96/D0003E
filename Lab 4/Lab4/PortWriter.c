#include <avr/io.h>
#include "TinyTimber.h"
#include "PortWriter.h"

#define PORTBIT4 0x10
#define PORTBIT6 0x40

/* Toggles a ports output bit */
void write(PortWriter *self, int arg) {
	if (arg == 4) {
		PORTE=PORTE^PORTBIT4;
		} else if (arg == 6) {
		PORTE=PORTE^PORTBIT6;
	}
}

/* Sets a ports output bit to 0 */
void deactivate(PortWriter *self, int arg) {
	if (arg == 4) {
		PORTE = PORTE & ~(PORTBIT4);
		} else if (arg == 6) {
		PORTE= PORTE & ~(PORTBIT6);
	}
}