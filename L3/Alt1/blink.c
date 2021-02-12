#include <avr/io.h>
#include "tinythreads.h"

/* Toggles a non-digit segment of the LCD with a frequency of 1 HZ */
void blink(void) {
	
	while(1) {
		if (readBlinkCounter()>=10) {
			/* Toggle non-digit segment */
			if (LCDDR3 == 0x0) {
				LCDDR3=0x1;
				} else {
				LCDDR3=0x0;
			}
			resetBlinkCounter();
		}
	}
}