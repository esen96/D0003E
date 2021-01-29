#include <avr/io.h>
#include "main.h"

int main(void) {
	
	// Disable CPU clock prescaler functionality
	CLKPR = 0x80;
	CLKPR = 0x00;

    initLCD();
    
	//writeChar('6',2);
	//writeLong(222137343);
	//primes();
	//blink();
	//button();
	long i = 2;
	runParts(i);
    
}
