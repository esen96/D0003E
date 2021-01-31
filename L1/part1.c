#include <avr/io.h>

// SCC character lookup table
static const uint16_t sccTable[10] = {
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

/* Outputs a number from 0-9 on the LCD */
void writeChar(char ch, int pos) {
	
	if (pos<0||pos>5) {
		return;
	}
		
	// Variable for holding SCC table lookup result
	uint16_t num;
	
	// Convert ch to int
	int chInt = ch-'0';
	if (chInt<0||chInt>9) {
		return;
		} else {
		num = sccTable[chInt];
	}
	
	// Nibble to be displayed on LCD
	uint8_t nib=0x0; 
	
	// LCDAddress variable initialized at LCDDR0
	uint8_t *LCDAddress=(uint8_t*)0xEC; 
	
	/* Modify LCDAddress to target the right pairwise address. 
	   Digit 0 and 1 at LCDDR0, 2 and 3 at LCDDR1, 4 and 5 at LCDDR2*/
	LCDAddress+=pos/2; 
	
	/* Set nibble mask for even and uneven LCD digits */
	uint8_t nibMask;
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

/* Outputs the six least significant digits of a given long value */
void writeLong(long i) {
	int pos=5;
	while (i>0) {
		char ch = (i%10)+'0';
		writeChar(ch, pos);
		i/=10;
		pos--;
	}
}

/* Outputs prime numbers to the LCD */
void primes(void) {
	for (long i=2;;i++){
		if (is_prime(i) == 1){
			writeLong(i);
		}
	}
}

/* Checks whether a given value is a prime number */
int is_prime(long i) {
	for (int n=2; n<i; n++){
		if (i%n == 0){
			return 0;
		}
	}
	return 1;
}
