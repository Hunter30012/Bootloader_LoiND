#include "MKL46Z4.h"
#include "UART.h"
#include "string.h"
#include "Flash.h"
#include "Queue.h"
#include "Move_VTOR.h"
#include "My_Lib.h"
#include "Srec.h"

/*****************************************
*Varibles
*****************************************/


/****************************************
*code
****************************************/

void main()
{
    /* phai nap chuong trinh App lan dau vao FLash bang cach bam nut va reset */
    /* Do S3 co dia chi vuot qua 4 thanh ghi nen khong xet toi */
    /* Init clock system 48MHz */
    MCG->C4 |= MCG_C4_DRST_DRS(1);
    MCG->C4 |= MCG_C4_DMX32_MASK;
    /***************************/
    uint8 u8status_SW1;
    initLed();
    u8status_SW1 = ( (GPIOC->PDIR ) & BUTTON_PRESS) >> BUTTON_SHIFT;
    if( u8status_SW1 == PRESS)
    {
        Bootloader_UART();
    }
    else
    {
        Run_App();
    }
    while(1)
    {
      
    }
}



