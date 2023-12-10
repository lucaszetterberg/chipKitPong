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

// Function whose purpose is to assign the initial scores of our player structs
void playerScoreInit(){
	player1.score = 0;
	player2.score = 0;
}

// Function whose purpose is to initliaze and call functions that are necessary for our menu (Menu Setup)
void menuInit(){
	display_string(1, "Welcome to Pong!");	
	display_update(0, x);	// For displaying the textBuffer 
	GAME_STATE = 0;			// Initializing gameState
	x = 0;					// Initializing menu selector x to keep track where we are in the menu
	HighScoreListIndex = 0; // Initializing HighScoreListIndex for being able to scroll in highScoreMenu
}
// Function whose purpose is to insert a highScore
void insertHighscore(Player *player1){		// Argument to acccess player1 score attribute
	clear_FrameBuffer(); 
	clear_textbuffer();
	display_string(0, "Your initials");

	HighScores[HighScoreIndex][3] = 58;	// Assigning ascii code for colon 
	HighScores[HighScoreIndex][4] = (player1->score / 10) + 48;		// Tens digit for score
	HighScores[HighScoreIndex][5] = (player1->score % 10) + 48;		// unit digit for score
	HighScores[HighScoreIndex][6] = 0x00;							// Null terminator
	

	int i = 0;

	delay(400000);	// Delay
	if(input(btns) == 4 && letterIndex < 3){		// If button 4 is pressed, it will change the letter you want to assign in alphabetical order
		letter++;	
	}

	if(input(btns) == 3 && letterIndex <= 2){
		textbuffer[1][letterIndex] = intToAscii(letter);
		HighScores[HighScoreIndex][letterIndex] = textbuffer[1][letterIndex];	// Here is when saving it to our HighScore array
		letterIndex++;

	}else if(input(btns) == 3){
		HighScoreIndex++;
		delay(10000);
		letterIndex = 0;
		playerScoreInit();	// Set player scores to zero for when you want to play again
		GAME_STATE = GAME_STATE_MENU;
	}          

	for(i = 0; i < 7; i++){		// Displays everyhting that has been chosen
		textbuffer[1][i] = HighScores[HighScoreIndex][i];
	}
	if(letterIndex < 3){
		textbuffer[1][letterIndex] = intToAscii(letter);	// Displaying current letter that you are choosing
	}

	display_update(0, x);	
}
// Function whose purpose is to setup game
void gameInit(){
	init_xorshift_seed();
	// Initializing ball
	ball.xPos = 63;
	ball.yPos = -15;
	ball.speed = 0.7;
	// Setting random ball trajectory each time we restart the game
	ball.dx = randomInt(-100, 100)/100.0; 
    ball.dy = randomInt(-100, 100)/100.0; 
	if(ball.dx < 30 && ball.dx > -30){	// Limiting the velocity to be either 5 or -5
		if(ball.dx > 0){
			ball.dx = 5.0;
		}else{
			ball.dx = -5.0;
		}
	} 

	float magnitude = custom_sqrt(ball.dx * ball.dx + ball.dy * ball.dy);
	ball.dx /= magnitude;	// Normalize the vectors to prevent the ball from gaining or losing velocity so the velocity can be constant
	ball.dy /= magnitude;
	ball.xPos = (ball.xPos + ball.speed * ball.dx);	 // Updating the ball's coordinates
    ball.yPos = (ball.yPos + ball.speed * ball.dy);

	ball.size = 2;	// Assigning ball size
	memcpy_custom(ball.image, Array, sizeof(Array));	// Copy memory block from ball.image to the destination memory block Array

	// Initializing paddles
	leftPaddle.xPos = 4;
    leftPaddle.yPos = -10;
    leftPaddle.width = 1;
    leftPaddle.height = 7;
	memcpy_custom(leftPaddle.image, Image, sizeof(Image));	// Similar logic applies here 

	rightPaddle.xPos = 123;
    rightPaddle.yPos = -10;
    rightPaddle.width = 1;
    rightPaddle.height = 7;
	memcpy_custom(rightPaddle.image, Image, sizeof(Image));	// Similar logic applies here
}
// Main function, this is where everything runs
int main(void) {
	// Call neccerssary functions
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
		btns = getbtns();	// Always read the status of the buttons and switches
		sw = getsw();

		if(IFS(0) & 0x100){ 				//If interrupt flag bit (bit 8) is 1, execute following code. 
			IFS(0) = (IFS(0) & 0xfffffeff); // Resets interrupt flag in each iteration
			T2CON = 0x0070; 				// Stops timer
			TMR2 = 0; 						// Resets timer to 0
	
			if(x > 3){	// Limit the pointer for the menu
				x = 0;
			}
			// Switch case depending on which gameState we are dealing with
			switch (GAME_STATE)
			{
			case GAME_STATE_MENU:	// When gameState is menu we call for menu with arguments btns
				mainMenu(btns);
				break;
			case GAME_STATE_ONE_PLAYER:	// Similarly here
				game(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
				break;
			case GAME_STATE_TWO_PLAYER: // Similarly here
				game(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
				break;
			case GAME_STATE_HIGHSCORE:	// Similarly here
				highscoreMenu();
				break;
			case GAME_STATE_INSERT_HIGHSCORE:	// Similarly here
				insertHighscore(&player1);
				break;
			case GAME_STATE_AI_MENU:	// Similarly here
				aiMenu();
				break;
			//*
			case GAME_STATE_SURVIVAL:	// Similarly here
				game(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
				break;
			//*/
			default:
				break;
			}


			sw = 0;		/* Restarts the values for the buttons and switches in order to not 
						get the impression that we are holding the buttons/switches */
			btns = 0;
			T2CON = 0x8070; // Starts timer
		}
	}
	return 0;
}
