#include <stdint.h>
#include <pic32mx.h>

int input(int btns){
    if(btns & 0x8){ // BTN1 pressed
        return 1;
    }

    if(btns & 0x1){ // BTN2 pressed
        return 2;
    }

    if(btns & 0x2){ // BTN3 pressed
        return 3;
    }

    if(btns & 0x4){ // BTN4 pressed
        return 4;
    }
    btns = 0;
}

int inputsw(int sw){
}
