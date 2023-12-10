#ifndef _Helpers
#define _Helpers

float custom_abs(float x);  // Function whose purpose is to return the absolute value of given argument
float custom_sqrt(float x); // Function whose purpose is to return the square root of given argument using the Newton - Raphson method
static void num32asc( char * s, int n );
uint32_t xorshift();
void init_xorshift_seed();
int intToAscii(int value);
int randomInt(int min, int max);

#endif