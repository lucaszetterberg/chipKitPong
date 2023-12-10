#include <stdint.h>
#include <pic32mx.h>
#include "gameAssets.h"
#include "gameStructs.h"
#include "displayControl.h"


// Function whose purpose is to assign the initial scores of our player structs
void playerScoreInit(Player *player1, Player *player2){
	player1 -> score = 0;
	player2 -> score = 0;
}

// Function whose purpose is to  copy a block of memory from a address to another address 
void* memcpy_custom(void *dest, const void *src, size_t n) {
    /* Dest represents a pointer pointing to the destination address
    and src is a pointer pointing to the source address 
    and n represents the number of bytes to copy*/
    char *d = dest;         // Initalizing dest pointer
    const char *s = src;    // Initializing src pointer 

    while (n--) {           // Loops the amount of times depending of the size we want to copy, n number of bytes
        *d++ = *s++;        /* Incrementing both pointers and assigning the src address byte to the dest address 
                            byte and will keep on doing this until the pointers traverses the whole block of memory*/
    }

    return dest;            // Then we return dest
}

// Function whose purpose is to draw our ball
void draw_ball(Ball *ball) {    // Takes in ball struct address
    int i;
    for (i = 0; i < 4; i++) {   // Loops through each pair of coortidinate pair
        coordToBuffer(ball->image[i][0] + ball->xPos, ball->image[i][1] + ball->yPos);
        /* Calls our coordToBuffer function to update our frameBuffer
        and the position is computed by adding the xPos of our ball struct to the coordinate in the image array
        in this way we compute it relative to the ball's current position, this is done the same for the yPos */
    }
}

// Function whose purpose is to render our paddle(s)
void draw_paddle(Paddle *paddle) {
    int i;
    for (i = 0; i < 7; i++) {   // Same method as ball
        coordToBuffer(paddle->image[i][0] + paddle->xPos, paddle->image[i][1] + paddle->yPos);
    }
}

// Function whose purpose is to update the given player score by incrementing the player structs 'score'
void score(Player *player){
    player->score += 1;
}

void reflectBall(Ball *ball, Paddle *paddle) {
    // Calculate the center of the paddle
    float paddleCenter = paddle->yPos - paddle->height / 2.0;

    // Calculate the relative position of the ball to the center of the paddle
    float relativePos = ball->yPos - paddleCenter;

    // Calculate the normalized reflection angle based on the relative position
    float normalizedAngle = relativePos / (paddle->height / 2.0);

    // Manually clamp the normalized angle to the range [-0.75, 0.75]
    if (normalizedAngle > 0.75) {
        normalizedAngle = 0.75;
    } else if (normalizedAngle < -0.75) {
        normalizedAngle = -0.75;
    }

    // Set the new direction of the ball
    ball->dx = -ball->dx;  // Invert the x-direction for reflection

    // Check if the ball hits the top or bottom part of the paddle
    if (ball->yPos > paddle->yPos && ball->yPos < (paddle->yPos - paddle->height)) {
        // Hit the bottom part of the paddle
        ball->dy = -(ball->dy + normalizedAngle); // Use the normalized angle for the y-direction
    } else {
        // Hit the top part of the paddle
        ball->dy = ball->dy + normalizedAngle;  // Use the normalized angle for the y-direction
    }
}

void ballMovement(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1) {

    float magnitude = custom_sqrt(ball -> dx * ball -> dx + ball -> dy * ball -> dy);
    // Normalize the vectors to prevent the ball from gaining or losing velocity so the velocity can be constant
	ball -> dx /= magnitude;    
	ball -> dy /= magnitude;
    // Updating the ball's position
	ball -> xPos = (ball -> xPos + ball -> speed * ball -> dx);
    ball -> yPos = (ball -> yPos + ball -> speed * ball -> dy);

	// Creating hitbox for ball
    float ballLeft = ball -> xPos;
    float ballRight = ball -> xPos + ball -> size;
    float ballTop = ball -> yPos;
    float ballBottom = ball -> yPos - ball -> size;

    // Creating hitboxes for the paddles
    float leftPaddleRight = (leftPaddle -> xPos) + (leftPaddle -> width);
    float leftPaddleTop = leftPaddle -> yPos;
    float leftPaddleBottom = leftPaddleTop - (leftPaddle -> height);

	float rightPaddleLeft = rightPaddle -> xPos;
    float rightPaddleTop = rightPaddle -> yPos;
    float rightPaddleBottom = rightPaddleTop - (rightPaddle -> height);

    // Handling collision with the upper and lower boundaries of the screen
    if ((ballTop >= 0 && ball -> dy > 0) || (ballBottom <= -28 && ball -> dy < 0)) {
        ball -> dy = -ball -> dy;
    }

    // Handling collisions with the paddles
	if (ballRight >= rightPaddleLeft && ballBottom <= rightPaddleTop && ballTop >= rightPaddleBottom && (ball -> dx) > 0 && (ball -> xPos) < ((rightPaddle -> xPos) + (rightPaddle -> width))){
		reflectBall(ball, rightPaddle);
		if(GAME_STATE == GAME_STATE_SURVIVAL){
			score(player1);
		}
		ball -> speed += 0.05;
    }

    if (ballLeft <= leftPaddleRight && ballBottom <= leftPaddleTop && ballTop >= leftPaddleBottom && (ball -> dx) < 0 && ((ball -> xPos) + (ball -> size)) > (leftPaddle -> xPos)){     // Checks left paddle
        reflectBall(ball, leftPaddle);
		ball -> speed += 0.05;
    }
}