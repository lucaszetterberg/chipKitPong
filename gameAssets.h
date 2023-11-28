#include <stdint.h>
#include <pic32mx.h>
#include <stdint.h>
#include <pic32mx.h>


typedef struct
{
    float xPos;
    float yPos;
    float size;
    float dx;
    float dy;
    float speed;
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
    int image[5][2];
} Paddle;

void draw_paddle(Paddle *paddle) {
    int i;
    for (i = 0; i < 5; i++) {
        coordToBuffer(paddle->image[i][0] + paddle->xPos, paddle->image[i][1] + paddle->yPos);
    }
}


typedef struct {
    int score;
} Player;

void score(Player *player){
    player->score += 1;
}
