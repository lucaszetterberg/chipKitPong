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
    /*
    int image[][] = {
        {0,0}, 
        {1,0}, 
        {2,0}, 
        {0,-1}, 
        {1,-1}, 
        {2,-1}, 
        {0,-2},
        {1,-2},
        {2,-1}};'
    */
} Ball;















/*
void draw_ball(ball){
    int i;
    for(i = 0; i < 10; i++){
        coordToBuffer(ball.image[i][0] + ball.xPos, ball.image[i][1] + ball.yPos);
    }
}
*/