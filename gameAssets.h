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

int GAME_STATE;
int AI_DIFFICULTY;
int btns;
int sw;
int x;
int letter;
int letterIndex;
char HighScores [8][7];
int HighScoreIndex;
int playerScore;
int HighScoreListIndex;
static uint32_t xorshift_seed;

void game(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, Player *player1, Player *player2);
void highscoreMenu();
void mainMenu();
void draw_ball(Ball *ball);
void* memcpy_custom(void *dest, const void *src, size_t n);
void draw_paddle(Paddle *paddle);
void score(Player *player);
void aiMenu();
void timersInit();
void reflectBall(Ball *ball, Paddle  *paddle);
void ai(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle);
void score_detection(Ball *ball, Player *player1, Player *player2);
void winner(Player *player1, Player *player2);



int Array[4][2];
int Image[7][2];

#endif