#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "displayControl.h"
#include "gameAssets.h"

#define GAME_STATE_MENU 0
#define GAME_STATE_ONE_PLAYER 1
#define GAME_STATE_TWO_PLAYER 2
#define GAME_STATE_HIGHSCORE 3
#define GAME_STATE_INSERT_HIGHSCORE 4

#define AI_EASY 1
#define AI_MEDIUM 2
#define AI_HARD 3


Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;
Player player1;
Player player2;


int GAME_STATE = 0;
int AI_DIFFICULTY;
int btns;
int sw;
int x = 0;
int letter;
int letterIndex;
char HighScores [10][5];
int HighScoreIndex;
int playerScore;
int HighScoreListIndex = 0;

static uint32_t xorshift_seed;


int Array[9][2] = {
	{0,0 }, {1,0 }, //{2,0 }, 
	{0,-1}, {1,-1} //{2,-1}, 
	//{0,-2}, {1,-2}, {2,-2}
};


int Image[7][2] = {
	{0,0},
	{0,-1},
	{0,-2},
	{0,-3},
	{0,-4},
	{0,-5},
	{0,-6}
};

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

  	//TRISD = (TRISD & 0xf07f) | 0x0f80;

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
}

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
			display_string(1, "Game starts!");
			display_update(0, x);
			delay(10000000);
			clear_textbuffer();
			playerScoreInit();
			letterIndex = 0;
			letter = 0;
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

void insertHighscore(){
	clear_FrameBuffer();
	clear_textbuffer();
	display_string(0, "Insert your initials");

	HighScores[HighScoreIndex][3] = 0x00;
	HighScores[HighScoreIndex][4] = player1.score + 48;
	//HighScores[HighScoreIndex][5] = player1.score % 10 + 48;
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

	for(i = 0; i < 5; i++){
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
			insertHighscore();  
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
	ball.speed = 2;
	ball.dx = randomInt(-100, 100)/100.0; // Random x position between 4 and 123
    ball.dy = randomInt(-100, 100)/100.0; // Random y position between -14 and 13
	ball.size = 2;
	memcpy_custom(ball.image, Array, sizeof(Array));

	// Initializing paddles
	leftPaddle.xPos = 0;
    leftPaddle.yPos = -10;
    leftPaddle.width = 1;
    leftPaddle.height = 7;
	memcpy_custom(leftPaddle.image, Image, sizeof(Image));

	rightPaddle.xPos = 127;
    rightPaddle.yPos = -10;
    rightPaddle.width = 1;
    rightPaddle.height = 7;
	memcpy_custom(rightPaddle.image, Image, sizeof(Image));

	AI_DIFFICULTY == AI_HARD;
}

void highscoreMenu(){
	clear_textbuffer();
	clear_FrameBuffer();

	
	int i, j;
	for(i = 0; i < 5; i++){
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

static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

void reflectBall(Ball *ball, Paddle *paddle) {
    // Calculate the center of the paddle
    float paddleCenter = (paddle->yPos - paddle->height / 2.0 + paddle->yPos) / 2.0;

    // Calculate the relative position of the ball to the center of the paddle
    float relativePos = ball->yPos - paddleCenter;

    // Calculate the normalized reflection angle based on the relative position
    float normalizedAngle = relativePos / (paddle->height / 2.0);

    // Manually clamp the normalized angle to the range [-0.25, 0.25]
    if (normalizedAngle > 0.25) {
        normalizedAngle = 0.25;
    } else if (normalizedAngle < -0.25) {
        normalizedAngle = -0.25;
    }

    // Set the new direction of the ball
    ball->dx = -ball->dx;  // Invert the x-direction for reflection
    ball->dy = normalizedAngle;  // Use the normalized angle for the y-direction
}


void ballMovement() {
    ball.xPos = (ball.xPos + ball.dx);
    ball.yPos = (ball.yPos + ball.dy);

	
    float ballLeft = ball.xPos;
    float ballRight = ball.xPos + ball.size;
    float ballTop = ball.yPos;
    float ballBottom = ball.yPos - ball.size;

    float paddleRight = leftPaddle.xPos + leftPaddle.width;
    float paddleTop = leftPaddle.yPos;
    float paddleBottom = leftPaddle.yPos - leftPaddle.height;

	float rightPaddleLeft = rightPaddle.xPos;
    float rightPaddleTop = rightPaddle.yPos;
    float rightPaddleBottom = rightPaddle.yPos - rightPaddle.height;

    if ((ballTop >= 0 && ball.dy > 0) || (ballBottom <= -28 && ball.dy < 0)) {
        ball.dy = -ball.dy;
    }

	if ((ballRight >= rightPaddleLeft && ballBottom <= rightPaddleTop && ballTop >= rightPaddleBottom && ball.dx > 0)){
		reflectBall(&ball, &rightPaddle);
    }

    if ((ballLeft <= paddleRight && ballBottom <= paddleTop && ballTop >= paddleBottom && ball.dx < 0)){                 // Checks left paddle
        reflectBall(&ball, &leftPaddle);
    }
}

void ai() {
	if(ball.xPos >= 63){
    // Adjust this value to control the AI's responsiveness
    float sensitivity = 0.05;

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

void game(){
	score_detection();
	winner();

	if(GAME_STATE == GAME_STATE_ONE_PLAYER){
		ai(&ball, &rightPaddle);
	}
    
    if(inputsw(sw) == 4){
		flush_display();
		gameInit();
        GAME_STATE = GAME_STATE_INSERT_HIGHSCORE;
    }

    clear_FrameBuffer();  
    ballMovement();
    draw_ball(&ball);
    draw_paddle(&leftPaddle);
    draw_paddle(&rightPaddle);
    display_image(FrameBuffer);

	if (GAME_STATE == GAME_STATE_ONE_PLAYER){
        switch(input(btns)){
			case 3: {
				if(!(leftPaddle.yPos <= -31 + leftPaddle.height)){
					leftPaddle.yPos--;
				}
				break;
			}
			case 4: {
				if(!(leftPaddle.yPos >= 0)){
					leftPaddle.yPos++;
				}
				break;
			}
    	}
    }


    if (GAME_STATE == GAME_STATE_TWO_PLAYER){
        switch(input(btns)){

        case 4: {
            if(!(leftPaddle.yPos >= 0)){
                leftPaddle.yPos++;
                break;
            }
        }

        case 3: {
            if(!(leftPaddle.yPos <= -31 + leftPaddle.height))
            leftPaddle.yPos--;
            break;
        }

        case 2: {
            if(!(rightPaddle.yPos >= 0)){
                rightPaddle.yPos++;
                break;
            }
        }

        case 1: {
            if(!(rightPaddle.yPos <= -31 + rightPaddle.height)){
                rightPaddle.yPos--;
                break;
                }
            }
        }
    }
}

int main(void) {
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
				game(AI_DIFFICULTY);
				break;
			case GAME_STATE_TWO_PLAYER:
				game(AI_DIFFICULTY);
				break;
			case GAME_STATE_HIGHSCORE:
				highscoreMenu();
				break;
			case GAME_STATE_INSERT_HIGHSCORE:
				insertHighscore();
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
