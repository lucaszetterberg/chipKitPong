#include <stdint.h>
#include <pic32mx.h>

int input(int btns){
    if(btns){
        switch (btns){
        case 4:    //BTN4 is pressed
        return 4;
        break;
        case 2:   //BTN3 is pressed
        return 3;
        break;
        case 1:   //BTN2 is pressed
        return 2;
        break;
        /*
        case 6:   //BTN 4 and 3 are pressed
        mytime = (sw << 8) | (sw << 12) | (mytime & 0x00ff);
        break;
        case 5:    //BTN 4 and 2 are pressed
        mytime = (sw << 4) | (sw << 12) | (mytime & 0x0f0f);
        break;
        case 3:    //BTN 3 and 2 are pressed
        mytime = (sw << 4) | (sw << 8) | (mytime & 0xf00f);
        break;
        case 7:
        mytime = (sw << 4) | (sw << 8) | (sw << 12) | (mytime & 0x000f);
        break;
        */
        }
        btns = 0;
    }
}
