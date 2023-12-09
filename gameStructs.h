#ifndef _Game_Structs
#define _Game_Structs


typedef struct
{
    float xPos;
    float yPos;
    int size;
    float dx;
    float dy;
    float speed;
    int image[4][2];  // Declare the array size here

} Ball;


typedef struct {
    int xPos;
    int yPos;
    int width;
    int height;
    int image[7][2];
} Paddle;


typedef struct {
    int score;
} Player;
#endif