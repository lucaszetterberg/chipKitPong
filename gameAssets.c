#include <stdint.h>
#include <pic32mx.h>
#include "gameAssets.h"
#include "gameStructs.h"
#include "displayControl.h"

void* memcpy_custom(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

void draw_ball(Ball *ball) {
    int i;
    for (i = 0; i < 4; i++) {
        coordToBuffer(ball->image[i][0] + ball->xPos, ball->image[i][1] + ball->yPos);
    }
}

void draw_paddle(Paddle *paddle) {
    int i;
    for (i = 0; i < 7; i++) {
        coordToBuffer(paddle->image[i][0] + paddle->xPos, paddle->image[i][1] + paddle->yPos);
    }
}

void score(Player *player){
    player->score += 1;
}