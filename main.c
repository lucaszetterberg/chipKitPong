#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "gameStructs.h"
#include "displayControl.h"
#include "gameAssets.h"

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;
Player player1;
Player player2;

static uint32_t xorshift_seed;

void playerScoreInit(){
	player1.score = 0;
	player2.score = 0;
	playerScore = 0x00;
}

void timer3Init(){
	T3CON = 0x0070;						// Makes bits 6-4 (TCKPS bits) to 1 (111) to scale the internal clock down by 256. See PIC32 family sheet page 56.
	IFSCLR(0) = 0x1000;					// Turns bit of interrupt flag to 0. It is bit 8 see PIC32 family sheet page 53. 
	PR3 = 31000; 					    // Calculates period by ((80 000 000/256)/PR2) = 0.001 => PR2 = 313
	TMR3 = 0; 							// Initializes timer2 to start at 0
	T3CON = 0x8070; 					// Starts timer (turns bit 15 to 1)
}

void timer2Init(){
	volatile unsigned *trise = (volatile int*) 0xbf886100;
  	*trise &= ~0xff;

	TRISDSET = 0x0f00;

	TRISFSET = 0x2;

	T2CON = 0x0070; 					// Makes bits 6-4 (TCKPS bits) to 1 (111) to scale the internal clock down by 256. See PIC32 family sheet page 56.
	IFS(0) = (IFS(0) & 0xfffffeff); 	// Turns bit of interrupt flag to 0. It is bit 8 see PIC32 family sheet page 53. 
	PR2 = 2000; 						// Calculates period by ((80 000 000/256)/PR2) = 0.001 => PR2 = 313
	TMR2 = 0; 							// Initializes timer2 to start at 0
	T2CON = 0x8070; 					// Starts timer (turns bit 15 to 1)
  
  	return;
}

void menuInit(){
	display_string(1, "Welcome to Pong!");
	display_update(0, x);
	GAME_STATE = 0;
	x = 0;
	HighScoreListIndex = 0;
}

void insertHighscore(Player *player1){
	clear_FrameBuffer();
	clear_textbuffer();
	display_string(0, "Your initials");

	//int temp;
	//temp = player1 -> score;

	HighScores[HighScoreIndex][3] = 0x00;
	HighScores[HighScoreIndex][4] = (player1->score/10) + 48;
	HighScores[HighScoreIndex][5] = (player1 -> score % 10) + 48;
	int i;

	delay(400000);
	if(input(btns) == 4 && letterIndex < 3){
		letter++;
	}

	if(input(btns) == 3 && letterIndex <= 2){
		HighScores[HighScoreIndex][letterIndex] = intToAscii();
		letterIndex++;


	}else if(input(btns) == 3){
		HighScoreIndex++;
		GAME_STATE = GAME_STATE_MENU;
	}          

	for(i = 0; i < 6; i++){
		textbuffer[1][i] = HighScores[HighScoreIndex][i];
	}

	textbuffer[1][letterIndex] = intToAscii();

	display_update(0, x);
}

void winner(){
	if(player1.score == 11 || player2.score == 11){

		display_string(1, "Player   wins!");
		display_string(2, "Toggle sw4 to continue");

		if(player1.score == 11){
			textbuffer[1][7] = 0x31;

		}else if(player2.score == 11){
			textbuffer[1][7] = 0x32;
		}
		
		display_update(0, x);
		while (!(PORTD >> 11));

		if(GAME_STATE == GAME_STATE_ONE_PLAYER){
			insertHighscore(&player1);  
		}else{
			GAME_STATE = GAME_STATE_MENU;
		}
		playerScoreInit();
	}
}

int intToAscii(){
	if(letter > 25){
		letter = 0;
	}
	return letter + 65;
}

void init_xorshift_seed() {
    // Use the system time as the seed
    while (!(IFS(0) & 0x1000)); // Wait for Timer2 overflow
	IFSCLR(0) = 0x1000;
    T3CON = 0x0070; // Disable Timer2
    xorshift_seed = TMR3; // Use Timer2 value as seed
	TMR3 = 0; 						// Resets timer to 0
	T3CON = 0x8070; 					// Starts timer (turns bit 15 to 1)
}

// Xorshift pseudo-random number generator
uint32_t xorshift() {
    xorshift_seed ^= xorshift_seed << 13;
    xorshift_seed ^= xorshift_seed >> 17;
    xorshift_seed ^= xorshift_seed << 5;
    return xorshift_seed;
}

// Generate a random integer in the range [min, max]
int randomInt(int min, int max) {
    // Ensure that min <= max
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    // Calculate the range, making sure to handle the full 32-bit range
    uint32_t range = (uint32_t)(max - min) + 1;

    // Use only the lower 32 bits for the random value
    return min + (xorshift() % range);
}

void gameInit(){
	init_xorshift_seed();
	// Initializing ball
	ball.xPos = 63;
	ball.yPos = -25;
	ball.speed = 1;

	ball.dx = randomInt(-100, 100)/100.0; // Random x position between 4 and 123
    ball.dy = randomInt(-100, 100)/100.0; // Random y position between -14 and 13
	float magnitude = custom_sqrt(ball.dx * ball.dx + ball.dy * ball.dy);
	ball.dx /= magnitude;
	ball.dy /= magnitude;
	ball.xPos = (ball.xPos + ball.speed * ball.dx);
    ball.yPos = (ball.yPos + ball.speed * ball.dy);

	ball.size = 2;
	memcpy_custom(ball.image, Array, sizeof(Array));

	// Initializing paddles
	leftPaddle.xPos = 4;
    leftPaddle.yPos = -10;
    leftPaddle.width = 1;
    leftPaddle.height = 7;
	memcpy_custom(leftPaddle.image, Image, sizeof(Image));

	rightPaddle.xPos = 123;
    rightPaddle.yPos = -10;
    rightPaddle.width = 1;
    rightPaddle.height = 7;
	memcpy_custom(rightPaddle.image, Image, sizeof(Image));

	AI_DIFFICULTY == AI_HARD;
}


static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
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

void ballMovement() {

    float magnitude = custom_sqrt(ball.dx * ball.dx + ball.dy * ball.dy);
	ball.dx /= magnitude;
	ball.dy /= magnitude;
	ball.xPos = (ball.xPos + ball.speed * ball.dx);
    ball.yPos = (ball.yPos + ball.speed * ball.dy);

	
    float ballLeft = ball.xPos;
    float ballRight = ball.xPos + ball.size;
    float ballTop = ball.yPos;
    float ballBottom = ball.yPos - ball.size;

    float leftPaddleRight = leftPaddle.xPos + leftPaddle.width;
    float leftPaddleTop = leftPaddle.yPos;
    float leftPaddleBottom = leftPaddle.yPos - leftPaddle.height;

	float rightPaddleLeft = rightPaddle.xPos;
    float rightPaddleTop = rightPaddle.yPos;
    float rightPaddleBottom = rightPaddle.yPos - rightPaddle.height;

    if ((ballTop >= 0 && ball.dy > 0) || (ballBottom <= -28 && ball.dy < 0)) {
        ball.dy = -ball.dy;
    }

	if (ballRight >= rightPaddleLeft && ballBottom <= rightPaddleTop && ballTop >= rightPaddleBottom && ball.dx > 0 && ball.xPos < rightPaddle.xPos + rightPaddle.width){
		reflectBall(&ball, &rightPaddle);
    }

    if ((ballLeft <= leftPaddleRight && ballBottom <= leftPaddleTop && ballTop >= leftPaddleBottom && ball.dx < 0 && ball.xPos + ball.size > leftPaddle.xPos)){                 // Checks left paddle
        reflectBall(&ball, &leftPaddle);
    }
}

void ai() {
	if(ball.xPos >= 63){
    // Adjust this value to control the AI's responsiveness
	float sensitivity;

	switch (AI_DIFFICULTY)
	{
	case AI_EASY:
		sensitivity = 0.05;
		break;
	case AI_MEDIUM:
		sensitivity = 0.1;
	case AI_HARD:
		sensitivity = 0.15;
	default:
		break;
	}

	float aiSpeed = AI_DIFFICULTY;
    // Calculate the difference between the ball's and paddle's Y positions
    float yDifference = ball.yPos - rightPaddle.yPos;

    // Adjust the paddle's Y position based on the difference
    rightPaddle.yPos += sensitivity * yDifference;

    // Ensure the paddle stays within bounds
    if (rightPaddle.yPos > 0) {
        rightPaddle.yPos = 0;
    } else if (rightPaddle.yPos < -26) {
        rightPaddle.yPos = -26;
    }
}
}


void score_detection(){
	if(ball.xPos <= 0){
		display_string(1, "Player 2 scores!");
		display_update(0, x);
		score(&player2);

		delay(10000000);

		if(GAME_STATE == GAME_STATE_ONE_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_ONE_PLAYER;
		}else if(GAME_STATE == GAME_STATE_TWO_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_TWO_PLAYER;
		}
	}

	if(ball.xPos >= 126){
		display_string(1, "Player 1 scores!");
		display_update(0, x);
		score(&player1);

		delay(10000000);

		if(GAME_STATE == GAME_STATE_ONE_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_ONE_PLAYER;
		}else if(GAME_STATE == GAME_STATE_TWO_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_TWO_PLAYER;
		}
	}
}

int count;

int main(void) {
	clear_FrameBuffer();
	spi_init();
	display_init();
	timer3Init();
	timer2Init();
	gameInit();
	menuInit();
	delay(10000000);

	while(1){
		delay(1000);
		btns = getbtns();
		sw = getsw();

		if(IFS(0) & 0x100){ 				//If interrupt flag bit (bit 8) is 1, execute following code. 
			IFS(0) = (IFS(0) & 0xfffffeff); // Resets interrupt flag in each iteration
			T2CON = 0x0070; 				// Stops timer
			TMR2 = 0; 						// Resets timer to 0
	
			if(x > 4){
				x = 1;
			}

			switch (GAME_STATE)
			{
			case GAME_STATE_MENU:
				mainMenu(btns);
				break;
			case GAME_STATE_ONE_PLAYER:
				game(&ball, &leftPaddle, &rightPaddle);
				break;
			case GAME_STATE_TWO_PLAYER:
				game(&ball, &leftPaddle, &rightPaddle);
				break;
			case GAME_STATE_HIGHSCORE:
				highscoreMenu();
				break;
			case GAME_STATE_INSERT_HIGHSCORE:
				insertHighscore(&player1);
				break;
			case GAME_STATE_AI_MENU:
				aiMenu();
				break;
			default:
				break;
			}


			sw = 0;
			btns = 0;
			T2CON = 0x8070; // Starts timer
		}
	}
	return 0;
}
