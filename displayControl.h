#ifndef _Display_Control
#define _Display_Control

#include <pic32mx.h>
#include <stdint.h>

// All of these declarations taken from labs
#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200
// Until here

#define OledDispMax 512	// Max number of bytes able to be stored 
#define colOledMax 128
#define rowOledMax 16
#define pageOledMax 4

uint8_t FrameBuffer[OledDispMax];
uint8_t EmptyBuffer[OledDispMax];

/** A delay function whose purpose is to make an delay of wanted time in milliseconds in the argument */
void delay(int cyc);

/** Function whose purpose is to clear our textbuffer, this is done by setting all pixels or in this case all elements
   in the 2D array to be set to 0 */
void clear_textbuffer();

/** This takes care of when wanting to receive data to the buffer or sending data from the buffer */
uint8_t spi_send_recv(uint8_t data);

/** Here we setup the SPI to work as intended, this is where we turn on the SPI for an example. In other words, here is where 
   the initializations takes place in order to setup the SPI */
void spi_init();

/** Here is where we intialize the display, this is done throug sending some commands through the SPI interface. For an example
   this can icnlude commands suchas as for turning on the display.*/
void display_init();

/** This function is introduced in order to manipulate a specific line in the text buffer and then display it.
    It fills out with spaces if the string is shorter than 16 characters */
void display_string(int line, char *s);

/** Renders a 4 page or line image on the display by using the SPI interface */
void display_image(uint8_t *data);

/** Flushes (clears) the display and also then updates the display */
void flush_display();

/** Clears the FrameBuffer by setting all elements in the 2D array to zeros (All the pixels), the method for clearing is similiar
    to the clear_textBuffer function */
void clear_FrameBuffer();

/** Sends data to the display, and which exact data is depending on what our framebuffer holds. It is also 
    responsible for updating each pixel on the screen. */
void display_update(int on, int x);

/* This is not to allocate more memory than neccesary since these are made in another file */
extern char textbuffer[4][16];
extern const uint8_t const font[128*8];

/** Renders one pixel on specified coordinates x and y. 
   The screen has its origin in the top left corner and 
   has its x and y axises according to it. */
void coordToBuffer(int x, int y);

#endif