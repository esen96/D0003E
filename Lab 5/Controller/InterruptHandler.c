#include <avr/io.h>
#include "InterruptHandler.h"


/* Interrupt function for simulator input */
void receive(InterruptHandler *self, int arg) {
	
	/* Wait for data to be received */
	while ( !(UCSR0A & (1 << RXC0)) );	
	
	uint8_t USARTreceive = UDR0;		// Read data written to the USART data register
	
	if (USARTreceive & 1) {
		ASYNC(self->controller, northboundArrival, 0);
	}
	else if ((USARTreceive >> 1) & 1) {
		ASYNC(self->controller, northboundEntry, 0);
	}
	else if ((USARTreceive >> 2) & 1) {
		ASYNC(self->controller, southboundArrival, 0);
	}
	else if ((USARTreceive >> 3) & 1) {
		ASYNC(self->controller, southboundEntry, 0);
	}
}

