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
}

int inputsw(int sw){

    if(sw & 0x1){ // Checks SW1
        return 1;
    }

    if(sw & 0x2){ // Checks SW2
        return 2;
    }

    if(sw & 0x4){ // Checks SW3
        return 3;
    }

    if(sw & 0x8){ // Checks SW4
        return 4;
    }
}
