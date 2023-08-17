#include "My_Lib.h"
#ifndef _UART_H
#define _UART_H

/****************************************************
*Defination
*****************************************************/
#define SBR_BDH_MASK        (0b1111100000000)
#define SBR_BDH_SHIFT       8
#define MAX_BDL             255


/****************************************************
*Prototypes
****************************************************/

void Print_char(char s);
/* Transmit char*/

void Print_string(char *s);
/* Transmit string */
void Print_string_array(uint8 *s, uint8 u8len);

char Receive_char();
/* receiver char */


static void SetBaudrate(uint32 u32baudrate, uint32 u32clockFreq);
/*
u32baudrate: baudrate setup
u32clockFreq: clock  for uartsrc
*/
void InitUART0(uint32 u32Baudrate);

void Receive_string(char *s);
/* Receive string */


#endif