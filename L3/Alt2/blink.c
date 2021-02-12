#include <avr/io.h>
#include "tinythreads.h"

/* Toggles a non-digit segment of the LCD */
void blink(void) {
	while(1) {
		lock(&blinkMutex);
		/* Toggle non-digit segment */
		if (LCDDR3 == 0x0) {
			LCDDR3=0x1;
			} else {
			LCDDR3=0x0;
		}
	}
}