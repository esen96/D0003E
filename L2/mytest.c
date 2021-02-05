#include "tinythreads.h"
#include "LCD.h"
#include "writechar.h"
#include <stdbool.h>

/* Checks whether a given value is a prime number */
bool is_prime(long i) {
	for (int n=2; n<i; n++){
		if (i%n == 0){
			return false;
		}
	}
	return true;
}

int pp;
mutex m;
void printAt(long num, int pos) {
	lock(&m);
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
	unlock(&m);
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
        }
    }
}

int main() {
	initLCD();
    spawn(computePrimes, 0);
    computePrimes(3);
}
