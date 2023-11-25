#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "displayControl.h"
#include "gameAssets.h"

Ball ball;

gameInit(){
	ball.xPos = 10;
	ball.yPos = -8;
	ball.dx = 1;
	ball.dy = -1;
}

ballMovement(){
	ball.xPos = (ball.xPos + ball.dx);
	ball.yPos = (ball.yPos + ball.dy);

	if(ball.xPos >= 127 || ball.xPos <= 0){
		ball.dx = -(ball.dx);
	}
	if(ball.yPos >= 0 || ball.yPos <= -31){
		ball.dy = -(ball.dy);
	}
}





game(){
	gameInit();
	int count;
	while(1){
		
		ballMovement();
		coordToBuffer(ball.xPos, ball.yPos);
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
