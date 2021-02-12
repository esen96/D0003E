#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"


#define FREQ 7813				/* 8000000/1024 */
#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                        *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

struct thread_block {
    void (*function)(int);   // code to run
    int arg;                 // argument to the above
    thread next;             // for use in linked lists
    jmp_buf context;         // machine state
    char stack[STACKSIZE];   // execution stack space
};

struct thread_block threads[NTHREADS];

struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

int initialized = 0;
int blinkCounter=0;

static void initialize(void) {
    int i;
    for (i=0; i<NTHREADS-1; i++) {
        threads[i].next = &threads[i+1];
	}
    threads[NTHREADS-1].next = NULL;

	/* Lab-specific settings */
	EIMSK = (1<<PCIE1);									/* Pin Change Interrupt Enable 1 */
	PCMSK1 = (1<<PCINT15);								/* Set PCINT15 mask*/
	PORTB = (1<<PB7);									/* Set bit 7 of port B to output */
	TCCR1A = (1<<COM1A1) | (1<<COM1A0);					/* Set OC1A to high on compare match  */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);		/* Set timer to CTC mode, set prescaler to 1024 */
	OCR1A = FREQ*0.05;									/* Frequency * 50 ms */
	TCNT1 = 0;											/* Clear TCNT1 register */
	TIMSK1 = (1<<OCIE1A);								/* Enable timer output compare A */
	
    initialized = 1;
}

static void enqueue(thread p, thread *queue) {
	p->next = NULL;
	if (*queue == NULL) {
		*queue = p;
	} else {
		thread q = *queue;
		while (q->next)
		q = q->next;
		q->next = p;
	}
}

static thread dequeue(thread *queue) {
    thread p = *queue;
    if (*queue) {
        *queue = (*queue)->next;
    } else {
        // Empty queue, kernel panic!!!
        while (1) ;  // not much else to do...
    }
    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context,1);
    }
}

void spawn(void (* function)(int), int arg) {
    thread newp;
	
    DISABLE();
    if (!initialized) initialize();

	newp = dequeue(&freeQ);
    newp->function = function;
    newp->arg = arg;
    newp->next = NULL;
    if (setjmp(newp->context) == 1) {
        ENABLE();
        current->function(current->arg);
        DISABLE();
        enqueue(current, &freeQ);
        dispatch(dequeue(&readyQ));
    }
    SETSTACK(&newp->context, &newp->stack);

    enqueue(newp, &readyQ);
	ENABLE();
}

void yield(void) {
	enqueue(current, &readyQ);
	dispatch(dequeue(&readyQ));
}

void lock(mutex *m) {
	DISABLE();
	if (m->locked==0) {
		m->locked=1;
	} else {
		enqueue(current, &(m->waitQ));
		dispatch(dequeue(&readyQ));
	}
	ENABLE();
}

void unlock(mutex *m) {
	DISABLE();
	if(m->waitQ) {
		enqueue(current, &readyQ);
		dispatch(dequeue(&(m->waitQ)));
	} else {
		m->locked=0;
	}
	ENABLE();
}

/* PCINT1_vect interrupt service routine */
ISR(PCINT1_vect) {
	if(PINB>>7==0){
		yield();
	}
}

/* TIMER1_COMPA_vect interrupt service routine */
ISR(TIMER1_COMPA_vect) {
	blinkCounter++;
	yield();
}

/* Returns the blink counter value */
int readBlinkCounter(void) {
	return blinkCounter;
}

/* Resets the blink counter value */
void resetBlinkCounter(void) {
	blinkCounter=0;
}
