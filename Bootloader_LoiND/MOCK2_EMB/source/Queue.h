#ifndef _Queue_H
#define _Queue_H

/****************************************************
*Defination
*****************************************************/
#define MAX_SIZE 50
#define MAX_ELEMENT_QUEUE 4
#define TRUE   1
#define FALSE  0

typedef struct
{
  uint8 u8length;
  uint8 buf[MAX_SIZE];
}buffer_data_t;

/****************************************************
*Prototypes
****************************************************/
__ramfunc uint8 CheckFull();
/* Check Queue  full?*/
__ramfunc buffer_data_t* GetWriteElementAdress();
/* Return adress of write_element*/
__ramfunc void Allocate_Element();
/* Give location */

uint8 CheckEmpty();
/* Check Queue  empty?*/
buffer_data_t* GetReadElementAdress();
/* Return adress of read_element*/
void FreeElement();
/* free locatuion */

#endif