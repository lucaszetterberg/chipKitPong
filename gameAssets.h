#ifndef _Game_Assets
#define _Game_Assets
#include "gameStructs.h"
#include "displayControl.h"
#include "helpers.h"

#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>

#define GAME_STATE_MENU 0
#define GAME_STATE_ONE_PLAYER 1
#define GAME_STATE_TWO_PLAYER 2
#define GAME_STATE_HIGHSCORE 3
#define GAME_STATE_INSERT_HIGHSCORE 4
#define GAME_STATE_AI_MENU 5
#define GAME_STATE_SURVIVAL 6

#define AI_EASY 1
#define AI_MEDIUM 2
#define AI_HARD 3

int GAME_STATE;     //Used for keeping track of the game state
int AI_DIFFICULTY;  //Used for keeping track of the difficulity of AI
int btns;           //Input signal from chipKit buttons
int sw;             //Input signal from chipKit switches
int x;              //Used to keep track of "selected" option in menus
int letter;         //Used to keep track of which letter is selected in insertHighScore menu
int letterIndex;    //Used to keep track of which letter index is selected in insertHighScore menu (ranges from 0-2)
char HighScores [8][7]; //2D array used to store highscores
int HighScoreIndex;     //Variable used to keep track of which index in HighScores[8][7] is going to be modified
//int playerScore;
int HighScoreListIndex; //Variable used to keep track of which row selected in Highscore menu
static uint32_t xorshift_seed;


/**
 * Game loop function, called from main function when any game mode is played
*/
void game(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1, Player *player2);

/**
 * Menu to view highscores
*/
void highscoreMenu();

/**
 * Main menu function, used when selecting game modes etc
*/
void mainMenu(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1, Player *player2);

/**
 * Adds input ball image to FrameBuffer
*/
void draw_ball(Ball *ball);

/**
 * Function whose purpose is to  copy a block of memory from a address to another address
*/
void* memcpy_custom(void *dest, const void *src, size_t n);

/**
 * Adds input paddle image to FrameBuffer
*/
void draw_paddle(Paddle *paddle);

/**
 * Increments input player score attribute by one
*/
void score(Player *player);

/**
 * Menu to choose AI, called after single player chosen
*/
void aiMenu();

/**
 * Initializes menu, only called in the beginning, displays "Welcome to Pong"
*/
void menuInit();

/**
 * Initializes game, called between rounds in game
*/
void gameInit(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle);

/**
 * Inserts highscore to HighScores list, called after game over in Survival mode
*/
void insertHighscore(Player *player1, Player *player2);

/**
 * Function whose purpose is to assign the initial scores of our player structs
*/
void playerScoreInit(Player *player1, Player *player2);

/**
 * Initializes timers 2 and 3
*/
void timersInit();
/**
 * Used for ball deflection when hitting paddle, ball deflect different amount depending on where
 * it hits the paddle
 * 
*/
void reflectBall(Ball *ball, Paddle  *paddle);
/**
 * 
*/
void ai(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle);
/**
 * Detects when the ball hits either the right or left side and gives the according 
 * player a point and initializes the game
 * 
*/
void score_detection(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1, Player *player2);
/**
 * Detects when one player has a score of 11 and declares that player as the winner and ends the game
 * 
*/
void winner(Player *player1, Player *player2);
/**
 * Used for implementing various ball physics inclding all bouncing. For convenience the function also gives the 
 * player score in the survival mode since the score in that mode is also dependent on the ball hitting the opponents paddle
 * 
*/
void ballMovement(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1);



int Array[4][2];
int Image[7][2];

#endif