#include <stdint.h>
#include <pic32mx.h>

typedef struct
{
    int xPos;
    int yPos;
    int size;
    int dx;
    int dy;
    int speed;

    int image[9][2];  // Declare the array size here

} Ball;

void draw_ball(Ball *ball) {
    int i;
    for (i = 0; i < 9; i++) {
        coordToBuffer(ball->image[i][0] + ball->xPos, ball->image[i][1] + ball->yPos);
    }
}

void* memcpy_custom(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

typedef struct {
    int xPos;
    int yPos;
    int width;
    int height;
    int imagePaddle[5][2];
} Paddle;
