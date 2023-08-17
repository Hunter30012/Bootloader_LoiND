#include "My_Lib.h"
#ifndef _FLASH_H
#define _FLASH_H

/****************************************************
*Defination
*****************************************************/
#define FCCOB1_MASK   0xFF0000
#define FCCOB2_MASK   0xFF00
#define FCCOB3_MASK   0xFF
#define FCCOB7_MASK   0xFF000000
#define FCCOB6_MASK   0xFF0000
#define FCCOB5_MASK   0xFF00
#define FCCOB4_MASK   0xFF

#define FCCOB1_SHIFT   16
#define FCCOB2_SHIFT   8
#define FCCOB7_SHIFT   24
#define FCCOB6_SHIFT   16
#define FCCOB5_SHIFT   8
#define SIZE_SECTOR    1024
#define MAX_FLASH     0x3FFFFF



/****************************************************
*Prototypes
****************************************************/
void Erase_Flash(uint32 u32Adress);


void Wrie2Flash(uint32 u32Adress, uint32 u32Data);


#endif