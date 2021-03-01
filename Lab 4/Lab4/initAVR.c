#include <avr/io.h>
#include "initAVR.h"
#include "LCD.h"

/* Initializes necessary drivers for the AVR, some drivers are already set in the TinyTimber kernel */
void initAVR() {
	
	initLCD();
	initInterrupt();
	
}


/* Sets LCD drivers */
void initLCD() {
	
	/* Set output voltage to 3.5 V, no need to set any LCDDC bit since 300 microseconds is three 0-bits. */
	LCDCCR = (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
	
	/* Use external asynchronous clock source, set MUX to 1/4 duty cycle, set LCD Port mask to enable 25 segments. */
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);

	/* Set low power waveform, enable LCD. */
	LCDCRA = (1 << LCDAB) | (1 << LCDEN);
	
	/* No LCD Prescaler Select bits set means N=16, LCD Clock divide bits set to select D=8. */
	LCDFRR = (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	
	/* Set initial indicator for pulse generator 1 */
	LCDDR3=0x1;
	
}

/* Sets interrupt drivers */
void initInterrupt() {
	
	/* Pin Change Interrupt Enable 1 and 0, PCINT7..0 are written to by PORT E  */
	EIMSK = (1 << PCIE1) | (1 << PCIE0);
	
	/* Enable interrupts for I/O pins 7, 6 and 4 on port B */
	PCMSK1 = (1 << PCINT15) | (1 << PCINT14) | (1 << PCINT12);
	
	/* Enable interrupts for I/O pins 3 and 2 on port E */ 
	PCMSK0 = (1 << PCINT3) | (1 << PCINT2); 
	
	/* Set bit 7, 6 and 4 of port B to output */
	PORTB = (1 << PB7) | (1 << PB6) | (1 << PB4);
	
	/* Set bit 3 and 2 of port E to output */
	PORTE = (1 << PE3) | (1 << PE2);
	
}
