#include "tinythreads.h"
#include "header.h"
#include <stdbool.h>

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


int main() {
	initLCD();
	spawn(blink, 0);
	spawn(button, 4);
    computePrimes(0);
}
