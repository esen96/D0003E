#include <avr/io.h>
#include "tinythreads.h"

/* Toggles a non-digit segment of the LCD */
void blink(int useless) {
	/* Toggle non-digit segment */
	if (LCDDR3 == 0x0) {
		LCDDR3=0x1;
	} else {
		LCDDR3=0x0;
	}
}