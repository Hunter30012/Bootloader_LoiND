#include "MKL46Z4.h"
#include "My_Lib.h"
#include "Queue.h"
#include "string.h"


/*****************************************
*Varibles
*****************************************/
uint8 g_u8front = 0;
uint8 g_u8rear;
uint8 g_u8used;
buffer_data_t g_array[MAX_ELEMENT_QUEUE];
buffer_data_t *write_adress;
/****************************************
*code
****************************************/
__ramfunc uint8 CheckFull()
{
    uint8 u8return;
    if( g_u8used < MAX_ELEMENT_QUEUE  )
    {
        u8return = 0;
    }
    else
    {
        u8return = 1;
    }
    return u8return;
}
__ramfunc buffer_data_t* GetWriteElementAdress()
{
    return  &g_array[g_u8rear];
}

__ramfunc void Allocate_Element()
{
    g_u8used++;
    g_u8rear = (g_u8rear + 1);
    if( g_u8rear == MAX_ELEMENT_QUEUE)
    {
        g_u8rear = 0;
    }
}

uint8 CheckEmpty()
{
    uint8 u8return;
    if( g_u8used == 0 )
    {
        u8return = 1;
    }
    else
    {
        u8return = 0;
    }
    return u8return;
}
buffer_data_t* GetReadElementAdress()
{
    return &g_array[g_u8front];
}

void FreeElement()
{
    g_u8front = (g_u8front + 1);
    if( g_u8front == MAX_ELEMENT_QUEUE)
    {
        g_u8front = 0;
    }
    g_u8used--;
}
__ramfunc void UART0_IRQHandler() /* move IRQ function to RAM */
{
    static uint8 u8index = 0;
    if(u8index == 0)
    {
    write_adress = &g_array[g_u8rear];
    }
    write_adress->buf[u8index] = UART0->D;
    u8index++;
    if( (write_adress->buf[u8index - 2] == '\r') && (write_adress->buf[u8index - 1] == '\n') ) /* \r and \n */
    {
        write_adress->u8length = u8index - 2;
        g_u8used++;
        g_u8rear = (g_u8rear + 1);
        if( g_u8rear == MAX_ELEMENT_QUEUE)
        {
            g_u8rear = 0;
        }
        u8index = 0;
        write_adress = &g_array[g_u8rear];
    }
}