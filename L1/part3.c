#include <avr/io.h>
#include <stdbool.h>

/* Switches between two non-digit segments on the LCD when the joystick is pushed downward */
void button(void) {
	
	// Set bit 7 of port B to output
	PORTB=0x80;
	
	// Initial LCD segment
	LCDDR13=0x1;
	
	bool risingEdge = false;
	
	while(1) {
		if (risingEdge == false && PINB>>7 == 0) {
			/* Switch LCD segment and set rising edge */
			if (LCDDR13 == 0x1) {
				LCDDR13=0x0;
				LCDDR18=0x1;
			} else {
				LCDDR13=0x1;
				LCDDR18=0x0;
			}
			risingEdge = true;
		
		} else if (PINB>>7 == 1) {
			risingEdge = false;
		}
	}
}
