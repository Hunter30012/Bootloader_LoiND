#include "MKL46Z4.h"
#include "My_Lib.h"
#include "Move_VTOR.h"
/*****************************************
*Varibles
*****************************************/
uint32 g_u32newVTB[VTB_SIZE] __attribute__ ((aligned (VTB_ALIGNMENT)));
/****************************************
*code
****************************************/
void MoveVTBtoRAM()
{
    uint8 i;
    uint32 *u32adr_VT;
    u32adr_VT = 0;
    for( i = 0; i< VTB_SIZE; i++)
    {
        g_u32newVTB[i] = *u32adr_VT;
        u32adr_VT++;
    }
    /* Move VTB */
    __asm("CPSID   I");
    SCB->VTOR = (uint32 )g_u32newVTB;
    __asm("CPSIE   I");
}