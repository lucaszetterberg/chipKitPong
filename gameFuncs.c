#include "gameAssets.h"


void game(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle){
	score_detection();
	winner();

	if(GAME_STATE == GAME_STATE_ONE_PLAYER){
		ai(ball, rightPaddle);
	}
    
    if(inputsw(sw) == 4){
		flush_display();
		gameInit();
        GAME_STATE = GAME_STATE_INSERT_HIGHSCORE;
    }

    clear_FrameBuffer();  
    ballMovement();
    draw_ball(ball);
    draw_paddle(leftPaddle);
    draw_paddle(rightPaddle);
    display_image(FrameBuffer);

	if (GAME_STATE == GAME_STATE_ONE_PLAYER){
        switch(input(btns)){
			case 3: {
				if(!(leftPaddle -> yPos <= (-31 + leftPaddle-> height))){
					leftPaddle -> yPos--;
				}
				break;
			}
			case 4: {
				if(!(leftPaddle -> yPos >= 0)){
					leftPaddle -> yPos++;
				}
				break;
			}
    	}
    }


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
}