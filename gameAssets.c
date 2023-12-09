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

void reflectBall(Ball *ball, Paddle  *paddle) {
    // Calculate the center of the paddle
    float paddleCenter = (paddle->yPos - paddle->height / 2.0 + paddle->yPos) / 2.0;

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