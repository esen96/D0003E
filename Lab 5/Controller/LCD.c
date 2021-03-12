#include <avr/io.h>
#include "LCD.h"

#define SOUTHPOS 0
#define BRIDGEPOS 2
#define NORTHPOS 4


/* SCC character lookup table */
static const uint16_t sccCharacters[10] = {
	0x1551, // 0
	0x0110, // 1
	0x1e11, // 2
	0x1B11, // 3
	0x0B50, // 4
	0x1B41, // 5
	0x1F41, // 6
	0x0111, // 7
	0x1F51, // 8
	0x0B51  // 9
};

/* Displays an SCC character on the LCD */
void writeChar(char ch, int pos) {
	
	if (pos<0||pos>5) {
		return;
	}
		
	uint16_t num;
	int chInt = ch-'0';
	if (chInt<0||chInt>9) {
		return;
		} else {
		num = sccCharacters[chInt];
	}

	// LCDAddress variable initialized at LCDDR0
	uint8_t *LCDAddress=(uint8_t*)0xEC; 
	
	/* Modify LCDAddress to target the right pairwise address. 
	   Digit 0 and 1 at LCDDR0, 2 and 3 at LCDDR1, 4 and 5 at LCDDR2*/
	LCDAddress+=pos/2; 
	
	uint8_t nib=0; 
	
	/* Set nibble mask for even and uneven LCD digits */
	uint16_t nibMask;
	if (pos%2 == 0) {
		nibMask=0xFFF0;
		} else {
		nibMask=0xFF0F;
	}
	
	/* Goes through the 16 bits retrieved from the SCC table lookup and 
	   outputs each nibble to their respective display segments */
	for (int i=0; i<4; i++) {
		nib = num&0x000F;		// Isolate nibble
		if(pos%2!=0) {
			nib = nib<<4;		// Odd numbers use high nibbles
		}	
		
		// Bitwise and with nibMask to preserve second digit
		*LCDAddress = (*LCDAddress & nibMask) | nib;
		
		// Shift away used nibble
		num = num>>4;

		// Increment to next digit display segment 
		LCDAddress+=5;
	}
	
}

/* Print number */
void printAt(long num, int pos) {
	int pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
}


/* Update north queue display data */
void updateNorth(LCD *self, int cars) {
	printAt(cars, NORTHPOS);
}

/* Update south queue display data */
void updateSouth(LCD *self, int cars) {
	printAt(cars, SOUTHPOS);
}

/* Update bridge display data */
void updateBridge(LCD *self, int cars) {
	printAt(cars, BRIDGEPOS);
}