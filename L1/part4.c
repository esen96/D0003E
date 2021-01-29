#include <avr/io.h>
#include <stdbool.h>

// The clock frequency, "fCLK_I/O/256" = 8000000/256
#define FREQ 31250

/* Runs parts 1, 2 and 3 sequentially */
void runParts(long i) {
	
	/* Part 2 settings and variables */
	TCCR1B = (1 << CS12);
	uint16_t nextValue=0;
	bool active = false;
	
	/* Part 3 settings and variables */
	PORTB=0x80;
	LCDDR13=0x1;
	bool risingEdge = false;
	
	while(1) {
		
		/* Part 1 */
		if (is_prime(i) == 1){
			writeLong(i);
		}
		i++;
		
		/* Part 2 with boolean and operator modifications to account for asynchrony */
		if (TCNT1 >= nextValue && !active) {
			active = true;
			nextValue+=(FREQ/2);
			if (LCDDR3 == 0x0) {
				LCDDR3=0x1;
				} else {
				LCDDR3=0x0;
			}
		}
		if (TCNT1 < nextValue) {
			active = false;
		}
		
		/* Part 3 */
		if (risingEdge == 0 && PINB>>7 == 0) {
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