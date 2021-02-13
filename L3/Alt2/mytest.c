#include "tinythreads.h"
#include "header.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define FREQ 7813				/* 8000000/1024 */

mutex blinkMutex={1,0};
mutex buttonMutex={1,0};

bool is_prime(long i) {
	for (int n=2; n<i; n++){
		if (i%n == 0){
			return false;
		}
	}
	return true;
}

void printAt(long num, int pos) {
    int pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
}

void computePrimes(int pos) {
    long n;
    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
        }
    }
}

void initTimer(void) {
	TCCR1A = (1<<COM1A1) | (1<<COM1A0);		/* Set OC1A to high on compare match  */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);	/* Set timer to CTC mode, set prescaler to 1024 */
	OCR1A = FREQ/2;					/* Generate interrupts two times per second */
	TCNT1 = 0;					/* Clear TCNT1 register */
	TIMSK1 = (1<<OCIE1A);				/* Enable timer output compare A */
}

void initButton(void) {
	EIMSK = (1<<PCIE1);				/* Pin Change Interrupt Enable 1 */
	PCMSK1 = (1<<PCINT15);				/* Set PCINT15 mask*/
	PORTB = (1<<PB7);				/* Set bit 7 of port B to output */
}

/* TIMER1_COMPA_vect interrupt service routine */
ISR(TIMER1_COMPA_vect) {
	unlock(&blinkMutex);
}

/* PCINT1_vect interrupt service routine */
ISR(PCINT1_vect) {
	unlock(&buttonMutex);
}


int main() {
	
	initLCD();
	initTimer();
	initButton();
	
	spawn(blink, 0);
	spawn(button, 4);
	
	computePrimes(0);
}
