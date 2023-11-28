#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "displayControl.h"
#include "gameAssets.h"

#define GAME_STATE_MENU 0
#define GAME_STATE_ONE_PLAYER 1
#define GAME_STATE_TWO_PLAYER 2
#define GAME_STATE_HIGHSCORE 3

#define AI_EASY 0
#define AI_MEDIUM 1
#define AI_HARD 2

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;
Player player1;
Player player2;
int GAME_STATE = 0;
int AI_DIFFICULTY;
int btns_temp;
int btns;
int sw;
int x = 0;


int Array[9][2] = {
        {0,0}, 
        {1,0}, 
        {2,0}, 
        {0,-1}, 
        {1,-1}, 
        {2,-1}, 
        {0,-2},
        {1,-2},
        {2,-2}};

int Image[5][2] = {
		{0,0},
		{0,-1},
		{0,-2},
		{0,-3},
		{0,-4}	
};

void timerInit(){
	volatile unsigned *trise = (volatile int*) 0xbf886100;
  	*trise &= ~0xff;

  	TRISD = (TRISD & 0xf07f) | 0x0f80;

	TRISFSET = 0x2;

	T2CON = 0x0070; // Makes bits 6-4 (TCKPS bits) to 1 (111) to scale the internal clock down by 256. See PIC32 family sheet page 56.
	IFS(0) = (IFS(0) & 0xfffffeff); //Turns bit of interrupt flag to 0. It is bit 8 see PIC32 family sheet page 53. 
	PR2 = 2000; // Calculates period by ((80 000 000/256)/PR2) = 0.001 => PR2 = 313
	TMR2 = 0; // Initializes timer2 to start at 0
	T2CON = 0x8070; //Starts timer (turns bit 15 to 1)
  
  return;
}

void menuInit(){
	display_string(1, "Welcome to Pong!");
	display_update(0, x);
}

void mainMenu(btns){
	//flush_display();
	display_string(1, "Single player");
	display_string(2, "Two Player");
	display_string(3, "Highscore");
	display_update(1, x);


	int mode = input(btns);
	switch (mode)
	{
	case 4:
		x += 1;
		delay(900000);
		break;
	case 3:
		switch (x)
		{
		case 1:
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			GAME_STATE = GAME_STATE_ONE_PLAYER;
			break;
		default:
			break;
		}
	case 1: 
		x += 1;
		delay(900000);
		break;

	default:
		break;
	}
	
}

void highscoreMenu(){
	display_string(1, "Single player");
	display_string(2, "Two Player");
	display_string(3, "Highscore");
	display_update(1, x);
}

		
void gameInit(){
	// Initializing ball
	ball.xPos = 10;
	ball.yPos = -14;
	ball.dx = 0.5;
	ball.dy = -0.5;
	memcpy_custom(ball.image, Array, sizeof(Array));

	// Initializing paddle
	leftPaddle.xPos = 6;
    leftPaddle.yPos = -10;
    leftPaddle.width = 1;
    leftPaddle.height = 5;
	memcpy_custom(leftPaddle.image, Image, sizeof(Image));

	rightPaddle.xPos = 100;
    rightPaddle.yPos = -10;
    rightPaddle.width = 1;
    rightPaddle.height = 5;
	memcpy_custom(rightPaddle.image, Image, sizeof(Image));

	// Initializing players
	player1.score = 0;
	player2.score = 0;

}

static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

void ballMovement() {
    ball.xPos = (ball.xPos + ball.dx);
    ball.yPos = (ball.yPos + ball.dy);

    float ballLeft = ball.xPos;
    float ballRight = ball.xPos + 3;
    float ballTop = ball.yPos;
    float ballBottom = ball.yPos - 3;

    //float paddleLeft = leftPaddle.xPos;
    float paddleRight = leftPaddle.xPos + leftPaddle.width;
    float paddleTop = leftPaddle.yPos;
    float paddleBottom = leftPaddle.yPos - leftPaddle.height;

	float rightPaddleLeft = rightPaddle.xPos;
    float rightPaddleRight = rightPaddle.xPos + rightPaddle.width;
    float rightPaddleTop = rightPaddle.yPos;
    float rightPaddleBottom = rightPaddle.yPos - rightPaddle.height;

    if (ballLeft <= 0 || ballRight >= 124) {
        ball.dx = -ball.dx;
    }

    if (ballTop <= 0 || ballBottom >= -28) {
        ball.dy = -ball.dy;
    }

	if ((ballRight >= rightPaddleLeft && ballBottom <= rightPaddleTop && ballTop >= rightPaddleBottom && ball.dx > 0)) // Checks right paddle
    {
        ball.dx = -ball.dx;
	}

	if ((ballLeft <= paddleRight && ballBottom <= paddleTop && ballTop >= paddleBottom && ball.dx < 0)) // Checks left paddle))
	{
		ball.dx = -ball.dx;
	}
}

/*
void ballMovement(){
	ball.xPos = (ball.xPos + ball.dx);
	ball.yPos = (ball.yPos + ball.dy);

	if(ball.xPos >= 124 || ball.xPos <= 0){
		ball.dx = -(ball.dx);
		if(ball.xPos >= 124){	// Player 2 (right side)
			score(&player2);
		}else{
			score(&player1);
		}
	}

	if(ball.yPos >= 0 || ball.yPos <= -28){
		ball.dy = -(ball.dy);
	}

	if (ball.xPos + 1 >= leftPaddle.xPos &&
        ball.xPos <= leftPaddle.xPos + leftPaddle.width &&
        ball.yPos >= leftPaddle.yPos &&
        ball.yPos < leftPaddle.yPos + leftPaddle.height) {
        ball.dx = -ball.dx;
    }
}
*/


int count;

void game(btns, sw, GAME_STATE, AI_DIFFICULTY){
	/*
	clear_textbuffer();
	num32asc(&textbuffer[0][6], player1.score);
	textbuffer[0][7] = 0x3A;
	num32asc(&textbuffer[0][8], player2.score);
	*/	


	if(sw & 0x8){
		GAME_STATE = GAME_STATE_MENU;
	}

	flush_display();
	//display_update(0, x);
	ballMovement();
	draw_ball(&ball);
	draw_paddle(&leftPaddle);
	draw_paddle(&rightPaddle);
	display_image(FrameBuffer);
}

int main(void) {
	spi_init();
	display_init();
	timerInit();
	gameInit();
	menuInit();
	delay(10000000);

	while(1){

		btns_temp = getbtns();
		sw = getsw();

		if(btns_temp > 0){
			btns = btns_temp;
		}



		if(IFS(0) & 0x100){ //If interrupt flag bit (bit 8) is 1, execute following code. 
			IFS(0) = (IFS(0) & 0xfffffeff); // Resets interrupt flag in each iteration
			T2CON = 0x0070; // Stops timer
			TMR2 = 0; // Resets timer to 0
			

			if(count == 16){
				flush_display();
				count = 0;
			}

			count++;

			if(x > 4){
				x = 1;
			}

			switch (GAME_STATE)
			{
			case GAME_STATE_MENU:
				mainMenu(btns);
				break;
			case GAME_STATE_ONE_PLAYER:
				game(btns, sw, GAME_STATE, AI_DIFFICULTY);
				break;
			default:
				break;
			}

			T2CON = 0x8070; // Starts timer
		}

		btns = 0;
	}
	return 0;
}