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

/*
typedef struct
{
    int xPos;
    int yPos;
    int size;
    int dx;
    int dy;
    int speed;
    int image[9][2];
    
} Ball;







void draw_ball(Ball *ball) {
    int i;
    for(i = 0; i < 10; i++){
        coordToBuffer( ball-> image + ball->xPos, ball->image[i][1] + ball->yPos);
    }
}
*/
