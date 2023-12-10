#include <pic32mx.h>

// Function whose purpose get state of the swithches of chipKit
int getsw( void ){
    int returnSW = (PORTD >> 8) & 0xf;  // Masks specific bits which holds the state
    return returnSW;
}

// Function whose purpose get state of the buttons of chipKit
int getbtns(void){
    int returnsBtns = ((PORTD >> 5) & 0x7) | ((PORTF << 2) & 0x8);  // Masks specific bits which holds the state

    return returnsBtns;
}