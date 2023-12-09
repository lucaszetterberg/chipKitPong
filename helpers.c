#include <stdint.h>
#include "gameAssets.h"

float custom_abs(float x) {
    return (x < 0) ? -x : x;
}

float custom_sqrt(float x) {
    if (x == 0.0 || x == 1.0) {
        return x;
    }

    float guess = x / 2.0;
    float epsilon = 0.0001;  // Adjust the epsilon value as needed for precision

    while (custom_abs(guess * guess - x) > epsilon) {
        guess = 0.5 * (guess + x / guess);
    }

    return guess;
}

static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

// Xorshift pseudo-random number generator
uint32_t xorshift() {
    xorshift_seed ^= xorshift_seed << 13;
    xorshift_seed ^= xorshift_seed >> 17;
    xorshift_seed ^= xorshift_seed << 5;
    return xorshift_seed;
}

void init_xorshift_seed() {
    // Use the system time as the seed
    while (!(IFS(0) & 0x1000)); // Wait for Timer3 overflow
	IFSCLR(0) = 0x1000;
    T3CON = 0x0070; // Disable Timer3
    xorshift_seed = TMR3; // Use Timer3 value as seed
	TMR3 = 0; 			  // Resets timer to 0
	T3CON = 0x8070; 	  // Starts timer (turns bit 15 to 1)
}

int intToAscii(int value){
	if(value > 25){
		value = 0;
	}
	return value + 65;
}

// Generate a random integer in the range [min, max]
int randomInt(int min, int max) {
    // Ensure that min <= max
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    // Calculate the range, making sure to handle the full 32-bit range
    uint32_t range = (uint32_t)(max - min) + 1;

    // Use only the lower 32 bits for the random value
    return min + (xorshift() % range);
}
