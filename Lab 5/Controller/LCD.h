#include "TinyTimber.h"

#ifndef LCD_H_
#define LCD_H_

typedef struct {
	Object super;
} LCD;

void writeChar(char ch, int pos);
void printAt(long num, int pos);
void updateNorth(LCD *self, int cars);
void updateSouth(LCD *self, int cars);
void updateBridge(LCD *self, int cars);

#define initLCD() { initObject() }

#endif /* LCD_H_ */