#include "gameAssets.h"


void mainMenu(){
	clear_textbuffer();
	display_string(1, "Single player ");
	display_string(2, "Two Player");
	display_string(3, "Highscore");
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
			GAME_STATE = GAME_STATE_HIGHSCORE;
			break;
		default:
			break;
		}
	default:
		break;
	}
	
}

void highscoreMenu(){
	clear_textbuffer();
	clear_FrameBuffer();

	
	int i, j;
	for(i = 0; i < 6; i++){
		textbuffer[HighScoreListIndex][i] = HighScores[HighScoreListIndex][i];
		textbuffer[HighScoreListIndex + 1][i] = HighScores[HighScoreListIndex + 1][i];
		textbuffer[HighScoreListIndex + 2][i] = HighScores[HighScoreListIndex + 2][i];
		textbuffer[HighScoreListIndex + 3][i] = HighScores[HighScoreListIndex + 3][i];
	}

	display_update(1, x);

	if(input(btns) == 4){
		if(x < 3){
			x++;
		}else{
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