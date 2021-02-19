#include <avr/io.h>

/* Initializes the LCD */
void initLCD(void) {
	
	// Set output voltage to 3.5 V, no need to set any LCDDC bit since 300 microseconds is three 0-bits.
	LCDCCR = (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
	
	// Use external asynchronous clock source, set MUX to 1/4 duty cycle, set LCD Port mask to enable 25 segments.
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);

	// Set low power waveform, enable LCD.
	LCDCRA = (1 << LCDAB) | (1 << LCDEN);
	
	// No LCD Prescaler Select bits set means N=16, LCD Clock divide bits set to select D=8.
	LCDFRR = (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	
}