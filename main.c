#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "displayControl.h"
#include "gameAssets.h"
#include <string.h>

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;


int Array[9][2] = {
        {0,0}, 
        {1,0}, 
        {2,0}, 
        {0,-1}, 
        {1,-1}, 
        {2,-1}, 
        {0,-2},
        {1,-2},
        {2,-1}};


void gameInit(){
	ball.xPos = 10;
	ball.yPos = -8;
	ball.dx = 1;
	ball.dy = -1;
	memcpy_custom(ball.image, Array, sizeof(Array));
	leftPaddle.xPos = 2;
    leftPaddle.yPos = -6;
    leftPaddle.width = 1;
    leftPaddle.height = 5;
    leftPaddle.image[0][0] = 0; leftPaddle.image[0][1] = 0;
    leftPaddle.image[1][0] = 0; leftPaddle.image[1][1] = -1;
    leftPaddle.image[2][0] = 0; leftPaddle.image[2][1] = -2;
    leftPaddle.image[3][0] = 0; leftPaddle.image[3][1] = -3;
    leftPaddle.image[4][0] = 0; leftPaddle.image[4][1] = -4;


}

void ballMovement(){
	ball.xPos = (ball.xPos + ball.dx);
	ball.yPos = (ball.yPos + ball.dy);

	if(ball.xPos >= 124 || ball.xPos <= 0){
		ball.dx = -(ball.dx);
	}
	if(ball.yPos >= 0 || ball.yPos <= -28){
		ball.dy = -(ball.dy);
	}

	if (ball.xPos == leftPaddle.xPos + leftPaddle.width &&
    ball.yPos >= leftPaddle.yPos && ball.yPos < leftPaddle.yPos + leftPaddle.height) {
    ball.dx = -ball.dx;
	
    }
}

void game(){
	gameInit();
	int count;
	while(1){

		ballMovement();
		coordToBuffer(ball.xPos, ball.yPos);
		draw_ball(&ball);
		display_image(0, FrameBuffer);
		flush_display();
		count = 0;

		delay(10001);
		count++;
	}

}

int main(void) {
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
        SPI2CON |= 0x60;
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000;

	display_init();
	/*
	//display_string(0, "such world");
	//display_string(1, "much hello");
	display_string(2, "many text");
	display_string(3, "wow");
	display_update();
	*/
	game();
	//coordToBuffer(1, -9);
	for(;;) ;
	return 0;
}
