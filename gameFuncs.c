#include "gameAssets.h"
void winner(Player *player1, Player *player2){
	if(player1 -> score == 11 || player2 -> score == 11){

		display_string(1, "Player   wins!");
        display_string(2, "Results xx:xx");
        textbuffer[2][8] = (player1->score / 10) + 48;
	    textbuffer[2][9] = (player1->score % 10) + 48;
        textbuffer[2][11] = (player2->score / 10) + 48;
	    textbuffer[2][12] = (player2->score % 10) + 48;
		display_string(3, "sw4 to main menu");

		if(player1 -> score == 11){
			textbuffer[1][7] = 0x31;

		}else if(player2 -> score == 11){
			textbuffer[1][7] = 0x32;
		}
		
		display_update(0, x);
		while (!(PORTD >> 11));

		if(GAME_STATE == GAME_STATE_ONE_PLAYER){
			insertHighscore(player1);  
		}else{
			GAME_STATE = GAME_STATE_MENU;
		}
		playerScoreInit();
	}
}
// Function whose purpose is to detect if a player has scored and which one of them
void score_detection(Ball *ball, Player *player1, Player *player2){
	if(ball -> xPos <= 0){      // If the ball manages to get past the paddle
        // Handles when this happens in gameState survival
        if(GAME_STATE == GAME_STATE_SURVIVAL){
            flush_display();
            clear_textbuffer();
            display_string(1, "    GAME OVER");     // Game over when ball manages to hit left side
            display_update(0, x);
            delay(1000000);
            gameInit();
            GAME_STATE = GAME_STATE_INSERT_HIGHSCORE;
            // Handles when this happens in gameState one player
        }else if(GAME_STATE == GAME_STATE_ONE_PLAYER){  
			display_string(1, "AI scores!");
		}else{
			display_string(1, "Player 2 scores!");
		}
		display_update(0, x);
		score(player2);     // Increment player2 score

		delay(10000000);
        // Setting up for new round
		if(GAME_STATE == GAME_STATE_ONE_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_ONE_PLAYER;
		}else if(GAME_STATE == GAME_STATE_TWO_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_TWO_PLAYER;
		}else if(GAME_STATE == GAME_STATE_SURVIVAL){
            gameInit();
			GAME_STATE = GAME_STATE_SURVIVAL;
        }
	}
    // Similar logic applies here
	if(ball -> xPos >= 126){
		display_string(1, "Player 1 scores!");
		display_update(0, x);
		score(player1);

		delay(10000000);

		if(GAME_STATE == GAME_STATE_ONE_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_ONE_PLAYER;
		}else if(GAME_STATE == GAME_STATE_TWO_PLAYER){
			gameInit();
			GAME_STATE = GAME_STATE_TWO_PLAYER;
		}else if(GAME_STATE == GAME_STATE_SURVIVAL){
            gameInit();
			GAME_STATE = GAME_STATE_SURVIVAL;
        }
	}
}
// Function whose purpose is to manage our Ai
void ai(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle) {
	if(ball -> xPos >= 63){
		// Adjust this value to control the AI's responsiveness
		float sensitivity;

        // Adjusting the sensivity depending on which difficulty the user chooses
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

        // Assigning our aiSpeed depending of which difficuly, the higher the difficulty the higher the velocity
		float aiSpeed = AI_DIFFICULTY;
		// Calculate the difference between the ball's and paddle's Y positions
		float yDifference = ball -> yPos - rightPaddle -> yPos;

		// Adjust the paddle's Y position based on the difference
		rightPaddle -> yPos += sensitivity * yDifference;

		// Ensure the paddle stays within bounds
		if (rightPaddle -> yPos > 0) {
			rightPaddle -> yPos = 0;
		} else if (rightPaddle -> yPos < -26) {
			rightPaddle -> yPos = -26;
		}
	}
}

// Function whose purpose is to start the game
// Takes five arguments, addresses ball and both the paddles and both players
void game(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1, Player *player2){ 
	winner(player1, player2); //If a player is declared as a winner it will end game loop

    // If the game state is one player we call for our ai function
	if(GAME_STATE == GAME_STATE_ONE_PLAYER || GAME_STATE == GAME_STATE_SURVIVAL){
		ai(ball, leftPaddle, rightPaddle);
	}

    // If specific switch is flicked the game ends
    if(inputsw(sw) == 4){
        flush_display();
        clear_textbuffer();
        display_string(1, "Rage quit?");
        display_update(0, x);
        delay(10000000);
		flush_display();
		gameInit();
        GAME_STATE = GAME_STATE_MENU;
    }

    /* Following lines we call for neccessary functions for displaying the wanted displau which for 
    an example includes drawing the objects such as our paddles */
    clear_FrameBuffer();  
    ballMovement(ball, leftPaddle, rightPaddle, player1);
    draw_ball(ball);
    draw_paddle(leftPaddle);
    draw_paddle(rightPaddle);
    display_image(FrameBuffer);

	if (GAME_STATE == GAME_STATE_ONE_PLAYER || GAME_STATE == GAME_STATE_SURVIVAL){
        switch(input(btns)){
			case 3: {
                /* For case 3 we move the left paddle downwards but limit the movement in order
                to keep it in the frame of the screen */
				if(!(leftPaddle -> yPos <= (-31 + leftPaddle-> height))){
					leftPaddle -> yPos--;
				}
				break;
			}
			case 4: {
				if(!(leftPaddle -> yPos >= 0)){ // Similar logic applies here
					leftPaddle -> yPos++;
				}
				break;
			}
    	}
    }

    /* Here we handle the case for multiplayer, here we have 4 cases for each button
    two for the leftPaddle and 2 for the rightPaddle */
    if (GAME_STATE == GAME_STATE_TWO_PLAYER){
        switch(input(btns)){

        case 4: {
            if(!(leftPaddle -> yPos >= 0)){
                leftPaddle -> yPos++;
                break;
            }
        }

        case 3: {
            if(!(leftPaddle -> yPos <= (-31 + leftPaddle -> height)))
            leftPaddle -> yPos--;
            break;
        }

        case 2: {
            if(!(rightPaddle -> yPos >= 0)){
                rightPaddle -> yPos++;
                break;
            }
        }

        case 1: {
            if(!(rightPaddle -> yPos <= (-31 + rightPaddle -> height))){
                rightPaddle -> yPos--;
                break;
                }
            }
        }
    }
    score_detection(ball, player1, player2);
}

void timersInit(){
	T3CON = 0x0070;						// Makes bits 6-4 (TCKPS bits) to 1 (111) to scale the internal clock down by 256. See PIC32 family sheet page 56.
	IFSCLR(0) = 0x1000;					// Turns bit of interrupt flag to 0. It is bit 8 see PIC32 family sheet page 53. 
	PR3 = 31000; 					    // Calculates period by ((80 000 000/256)/PR2) = 0.001 => PR2 = 313
	TMR3 = 0; 							// Initializes timer2 to start at 0
	T3CON = 0x8070; 					// Starts timer (turns bit 15 to 1)

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