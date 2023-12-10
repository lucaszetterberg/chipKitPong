#include "gameAssets.h"

// Function for mainMenu
void mainMenu(){
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
		//*
		case 0:		// When you are in first page and press button 3 the survival mode will start
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			playerScoreInit();
			letterIndex = 0;
			letter = 0;
			AI_DIFFICULTY = AI_MEDIUM;
			GAME_STATE = GAME_STATE_SURVIVAL;
			break;
		//*/
		case 1:			// Similar logic applies here
			clear_textbuffer();
			btns = 0;
			sw = 0;
			delay(900000);
			GAME_STATE = GAME_STATE_AI_MENU;
			break;
		case 2:
			clear_textbuffer();
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			playerScoreInit();
			letterIndex = 0;
			letter = 0;
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
		gameInit();
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
			playerScoreInit();
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
			playerScoreInit();
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
			playerScoreInit();
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