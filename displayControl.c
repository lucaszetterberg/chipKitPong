#include <pic32mx.h>
#include <stdint.h>
#include "displayControl.h"

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

void clear_textbuffer(){
	int i,j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 16; j++){
			textbuffer[i][j] = 0x00;
		}
	}
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08)); // Waits until the buffer is empty
	SPI2BUF = data; // When it is, it fills the buffer with 'data'
	while(!(SPI2STAT & 0x01)); // Waits until the buffer is full, it checks the status,
	return SPI2BUF; // Then returns the data in when it is full
}

void spi_init() {
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

}

void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4) 			// Firstly we check that the given argument 'line' is within bounds
		return;
	if(!s) 								// Furthermore, we check if the given pointer 's' is null
		return;
	
	for(i = 0; i < 16; i++) 			// We then iterate through the given line horizontally across the screen
		if(*s) { 						// If the given pointer is not null
			textbuffer[line][i] = *s; 	// We set each specific element to what value the pointer holds
			s++; 						// Increment the pointer in order to move to the next character in the string 
		} else
			textbuffer[line][i] = ' '; /* Otherwise if the current character is the null terminator we know that the char has traversed
										  through the string entirely and we then can fill the remaining elements with spaces */
}

void display_image(uint8_t *data) { 		// Pointer to an array which contations data for the whole display
	int i, j;
	
	for(i = 0; i < 4; i++) { 				// Loop for the 4 pages
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK; // Sets it to command mode
		spi_send_recv(0x22);				// Tells spi that subsequent data will be sent to the specified page
		spi_send_recv(i);					// Tells it which page that will be manipulated
		
		spi_send_recv(0x00);				// These 2 rows sets in which column addrewss the data will be sent to
		spi_send_recv(0x10);

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK; /* Sets the command to data mode, in order to treat the given data
																	as display infoirmation rather than commands */
		
		for(j = 0; j < 128; j++) 			// Iterates through each column in the current row
			spi_send_recv(data[i*128 + j]); // Sends the data from the current column in 'data' to the current column
	}
}

void flush_display() { // Similar to the previous function but this one flushes the display
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0x00);
        spi_send_recv(0x10);

        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        for (j = 0; j < 128; j++)
            FrameBuffer[i * 128 + j] = 0x00;

        for (j = 0; j < 128; j++)
            spi_send_recv(FrameBuffer[i * 128 + j]);
    }
}

void clear_FrameBuffer(){ 						// Clears the frameBuffer
	int i, j;

    for (i = 0; i < 4; i++) { 				 	// Iterate through each page
        for (j = 0; j < 128; j++) 			 	// Iterathe through the 128 elements horizontally 
            FrameBuffer[i * 128 + j] = 0x00; 	// Setting each element to be zero which in simpler terms means that we clear it
    }
}

void display_update(int on, int x) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) { 	// Loop through the 4 pages
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK; //Sets it to command mode
		spi_send_recv(0x22);	// Tells spi that subsequent data will be sent to the specified page
		spi_send_recv(i);		// Tells it which page that will be manipulated
		
		spi_send_recv(0x0);		// These 2 rows sets in which column addrewss the data will be sent to
		spi_send_recv(0x10);
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK; /* Sets the command to data mode, in order to treat the given data
																	as display infoirmation rather than commands */
		for(j = 0; j < 16; j++) {		// Loops through the 2D textBuffer
			c = textbuffer[i][j];		
			if(c & 0x80)		/* Every charcter is handled in such a way that each one of them contains 8 bit (1 byte)
									and in this case if the most significant bit is set we continue */
				continue;
			
			for(k = 0; k < 8; k++)	// Iterates through the 8 bits that each character consist of
				if(i == x % 4 && on == 1){	// Checks if the current pagematches X and if ON is set to 1 (On has the function similar to a flag)
					spi_send_recv(~font[c*8 + k]);	// If the condition are met we invert the font
				}else{
					spi_send_recv(font[c*8 + k]);	// Else keep it the same way, same font
				}

		}
	}
}

/* Renders one pixel on specified coordinates x and y. 
   The screen has its origin in the top left corner and 
   has its x and y axises according to it. */
void coordToBuffer(int x, int y) {
    short offset = 0;
	uint8_t positiveY = -(y);

	offset = positiveY / 8; 

	FrameBuffer[offset * 128 + x] |= 1 << (positiveY - offset * 8);
}