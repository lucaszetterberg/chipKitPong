#include <pic32mx.h>

int getsw( void ){
    int returnSW = (PORTD >> 8) & 0xf;
    return returnSW;
}

int getbtns(void){
    int returnsBtns = (PORTD >> 5) & 0x7;
    return returnsBtns;
}