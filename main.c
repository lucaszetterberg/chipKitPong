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

void playerScoreInit(){
	player1.score = 0;
	player2.score = 0;
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

	HighScores[HighScoreIndex][3] = 58;
	HighScores[HighScoreIndex][4] = (player1->score / 10) + 48;
	HighScores[HighScoreIndex][5] = (player1->score % 10) + 48;
	HighScores[HighScoreIndex][6] = 0x00;
	

	int i = 0;

	delay(400000);
	if(input(btns) == 4 && letterIndex < 3){
		letter++;
	}

	if(input(btns) == 3 && letterIndex <= 2){
		textbuffer[1][letterIndex] = intToAscii(letter);
		HighScores[HighScoreIndex][letterIndex] = textbuffer[1][letterIndex];
		letterIndex++;

	}else if(input(btns) == 3){
		HighScoreIndex++;
		delay(10000);
		letterIndex = 0;
		playerScoreInit();
		GAME_STATE = GAME_STATE_MENU;
	}          

	for(i = 0; i < 7; i++){
		textbuffer[1][i] = HighScores[HighScoreIndex][i];
	}
	if(letterIndex < 3){
		textbuffer[1][letterIndex] = intToAscii(letter);
	}

	display_update(0, x);
}

void gameInit(){
	init_xorshift_seed();
	// Initializing ball
	ball.xPos = 63;
	ball.yPos = -15;
	ball.speed = 0.7;

	ball.dx = randomInt(-100, 100)/100.0; 
    ball.dy = randomInt(-100, 100)/100.0; 
	if(ball.dx < 30 && ball.dx > -30){
		if(ball.dx > 0){
			ball.dx = 5.0;
		}else{
			ball.dx = -5.0;
		}
	} 

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
}

int main(void) {
	clear_FrameBuffer();
	clear_textbuffer();
	spi_init();
	display_init();
	timersInit();
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
	
			if(x > 3){
				x = 0;
			}

			switch (GAME_STATE)
			{
			case GAME_STATE_MENU:
				mainMenu(btns);
				break;
			case GAME_STATE_ONE_PLAYER:
				game(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
				break;
			case GAME_STATE_TWO_PLAYER:
				game(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
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
			//*
			case GAME_STATE_SURVIVAL:
				game(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
				break;
			//*/
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
