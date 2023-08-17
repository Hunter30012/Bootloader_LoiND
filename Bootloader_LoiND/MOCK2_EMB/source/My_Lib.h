#ifndef _MY_LIB_H
#define _MY_LIB_H

/****************************************************
*Defination
*****************************************************/
#define BUTTON_PRESS  (1<<3)
#define BUTTON_SHIFT   3
#define PRESS    0
#define RELEASE  1
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


/****************************************************
*Prototypes
****************************************************/
void initLed();
/* Init Led and Button */

void Bootloader_UART();
/* Send srec to run app */

void Run_App();
/* Run app */
#endif