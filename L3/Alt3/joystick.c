#include <avr/io.h>
#include <stdbool.h>
#include "header.h"
#include "tinythreads.h"

long pressCount = 0;
bool risingEdge = false;

/* Displays how many times the joystick has been pressed down on a given LCD position */
void button(int pos) {
	if (risingEdge == false && PINB>>7 == 0) {
		pressCount++;
		printAt(pressCount, pos);
		risingEdge = true;
	} else if (PINB>>7 == 1) {
		risingEdge = false;
	}
}