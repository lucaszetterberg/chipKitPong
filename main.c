#include <pic32mx.h>
#include <stdint.h>
#include "gameStructs.h"
#include "displayControl.h"
#include "gameAssets.h"

/**
 * Game made by:
 * Dilan Saleh
 * Lucas Zetterberg
 * 
*/

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;
Player player1;
Player player2;

// Main function, this is where everything runs
int main(void) {
	// Call neccerssary functions
	clear_FrameBuffer();
	clear_textbuffer();
	spi_init();
	display_init();
	timersInit();
	gameInit(&ball, &leftPaddle, &rightPaddle);
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
				mainMenu(&ball, &leftPaddle, &rightPaddle, &player1, &player2);
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
				insertHighscore(&player1, &player2);
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
