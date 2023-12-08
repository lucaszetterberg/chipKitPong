#ifndef _Display_Control
#define _Display_Control

#include <pic32mx.h>
#include <stdint.h>

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

#define OledDispMax 512	// Max number of bytes able to be stored 
#define colOledMax 128
#define rowOledMax 16
#define pageOledMax 4

uint8_t FrameBuffer[OledDispMax];
uint8_t EmptyBuffer[OledDispMax];

void delay(int cyc);
void clear_textbuffer();
uint8_t spi_send_recv(uint8_t data);
void spi_init();
void display_init();
void display_string(int line, char *s);
void display_image(uint8_t *data);
void flush_display();
void clear_FrameBuffer();
void display_update(int on, int x);


extern char textbuffer[4][16];
extern const uint8_t const font[128*8];
extern 

/**
 * Renders one pixel on specified coordinates x and y. 
 * The screen has its origin in the top left corner and 
 * has its x and y axises according to it.
 *
 * Input: int x, int y (pixel coordinates)
 * 
 * Output: None
*/
void coordToBuffer(int x, int y);

#endif