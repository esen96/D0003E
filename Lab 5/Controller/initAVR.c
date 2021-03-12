#include <avr/io.h>
#include "initAVR.h"
#include "LCD.h"

#define FOSC 8000000 // Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

/* Initializes necessary drivers for the AVR, some drivers are already set in the TinyTimber kernel */
void initAVR() {
	
	LCD_init();
	USART_init( MYUBRR );

}


/* Initialize LCD drivers */
void LCD_init() {
	
	/* Set output voltage to 3.5 V, no need to set any LCDDC bit since 300 microseconds is three 0-bits. */
	LCDCCR = (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
	
	/* Use external asynchronous clock source, set MUX to 1/4 duty cycle, set LCD Port mask to enable 25 segments. */
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);

	/* Set low power waveform, enable LCD. */
	LCDCRA = (1 << LCDAB) | (1 << LCDEN);
	
	/* No LCD Prescaler Select bits set means N=16, LCD Clock divide bits set to select D=8. */
	LCDFRR = (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	
	/* Print initial traffic data */
	printAt(00, 0); printAt(00, 2); printAt(00, 4);
	
}

/* Initialize USART drivers */
void USART_init( unsigned int ubrr ) {
	
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/* Enable receive interrupts, enable receiver, enable transmitter */
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	/* Set transmitter and receiver data size to 8 bits */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
}
