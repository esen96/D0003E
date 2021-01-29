#include <avr/io.h>

// The clock frequency, "fCLK_I/O/256" = 8000000/256
#define FREQ 31250

/* Toggles a non-digit segment of the LCD with a frequency of 1 HZ */
void blink(void) {
	
	// Set prescaling factor to 256
	TCCR1B = (1 << CS12);
	
	// Next value variable
	uint16_t nextValue=0;
	
	while(1) {
		if (TCNT1 == nextValue) {
			nextValue+=(FREQ/2);
			
			/* Toggle non-digit segment */
			if (LCDDR3 == 0x0) {
				LCDDR3=0x1;
				} else {
				LCDDR3=0x0;
			}
		}
	}
}