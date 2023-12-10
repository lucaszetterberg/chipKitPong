#include "gameAssets.h"

// Function whose purpose is to initliaze and call functions that are necessary for our menu (Menu Setup)
void menuInit(){
	display_string(1, "Welcome to Pong!");	
	display_update(0, x);	// For displaying the textBuffer 
	GAME_STATE = 0;			// Initializing gameState
	x = 0;					// Initializing menu selector x to keep track where we are in the menu
	HighScoreListIndex = 0; // Initializing HighScoreListIndex for being able to scroll in highScoreMenu
}

// Function whose purpose is to insert a highScore
void insertHighscore(Player *player1, Player *player2){		// Argument to acccess player1 score attribute (player2 for player init call)
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
		playerScoreInit(player1, player2);	// Set player scores to zero for when you want to play again
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
// Function for mainMenu
void mainMenu(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1, Player *player2){
	clear_textbuffer();
	display_string(0, "Survival Mode");
	display_string(1, "Single player");
	display_string(2, "Two Player");
	display_string(3, "Highscore");
	display_update(1, x);		// Has one as input in order to invert the current selected page selected by x

	int mode = input(btns);	
	int sw4 = inputsw(sw);

	switch (mode)
	{
	case 4:
		x += 1;		// Traverse in menu when button 4 is pressed
		delay(900000);
		break;
	case 3:		// When button 3 is pressed you select the current page that you are selecting in X
		switch (x)
		{

		case 0:		// When you are in first page and press button 3 the survival mode will start
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			letterIndex = 0;
			letter = 0;
			gameInit(ball,leftPaddle, rightPaddle);
			AI_DIFFICULTY = AI_MEDIUM;
			GAME_STATE = GAME_STATE_SURVIVAL;
			break;
		case 1:			// Similar logic applies here
			clear_textbuffer();
			btns = 0;
			sw = 0;
			delay(900000);
			gameInit(ball,leftPaddle, rightPaddle);
			GAME_STATE = GAME_STATE_AI_MENU;
			break;
		case 2:
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			letterIndex = 0;
			letter = 0;
			gameInit(ball,leftPaddle, rightPaddle);
			GAME_STATE = GAME_STATE_TWO_PLAYER;
			break;
		case 3:
			x = 0;
			GAME_STATE = GAME_STATE_HIGHSCORE;
			break;
		default:
			break;
		}
	default:
		break;
	}
	
}
// Function for highScore menu
void highscoreMenu(){
	clear_textbuffer();
	clear_FrameBuffer();

	// Displays content for highScores that have been saved
	int i;
	for(i = 0; i < 7; i++){
		textbuffer[0][i] = HighScores[HighScoreListIndex][i];
		textbuffer[1][i] = HighScores[HighScoreListIndex + 1][i];
		textbuffer[2][i] = HighScores[HighScoreListIndex + 2][i];
		textbuffer[3][i] = HighScores[HighScoreListIndex + 3][i];
	}
	// 
	if(input(btns) == 4){
		if(x < 3){
			x++;
		}else if(HighScoreListIndex >= 5 && x >= 3){
			x = 0;
			HighScoreListIndex = 0;

		}else if(HighScoreListIndex < 5){
			HighScoreListIndex++;

		}
		delay(900000);
	}

	if(inputsw(sw) == 4){
		HighScoreListIndex = 0;
		x = 0;
		clear_FrameBuffer();
		clear_textbuffer();
        GAME_STATE = GAME_STATE_MENU;
    }

	display_update(1, x);
	
}



void aiMenu(){
	clear_textbuffer();
	display_string(1, "Easy");
	display_string(2, "Medium");
	display_string(3, "Hard");
	display_update(1, x);

	int mode = input(btns);
	int sw4 = inputsw(sw);

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
			letterIndex = 0;
			letter = 0;
			AI_DIFFICULTY = AI_EASY;
			GAME_STATE = GAME_STATE_ONE_PLAYER;
			break;
		case 2:
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			letterIndex = 0;
			letter = 0;
			AI_DIFFICULTY = AI_MEDIUM;
			GAME_STATE = GAME_STATE_ONE_PLAYER;
			break;
		case 3:
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			letterIndex = 0;
			letter = 0;
			AI_DIFFICULTY = AI_HARD;
			GAME_STATE = GAME_STATE_ONE_PLAYER;
			break;
		default:
			break;
		}
	default:
		break;
	}

}