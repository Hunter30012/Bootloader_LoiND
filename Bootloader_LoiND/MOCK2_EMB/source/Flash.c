#include "MKL46Z4.h"
#include "My_Lib.h"
#include "Flash.h"

/*****************************************
*Varibles
*****************************************/

/****************************************
*code
****************************************/
void Erase_Flash(uint32 u32Adress)
{
    uint8 u8FCCOB1;
    uint8 u8FCCOB2;
    uint8 u8FCCOB3;
    u8FCCOB1 = (u32Adress & FCCOB1_MASK) >> FCCOB1_SHIFT;
    u8FCCOB2 = (u32Adress & FCCOB2_MASK) >> FCCOB2_SHIFT;
    u8FCCOB3 = u32Adress & FCCOB3_MASK;
    
    if( (u32Adress % SIZE_SECTOR == 0) && (u32Adress <= MAX_FLASH)  )
    {
        while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) != FTFA_FSTAT_CCIF_MASK)
        {
        }
         /* Adress */
        FTFA->FCCOB3 = u8FCCOB3;
        FTFA->FCCOB2 = u8FCCOB2;
        FTFA->FCCOB1 = u8FCCOB1;
        /* Command */
        FTFA->FCCOB0 = FTFA_FCCOB0_CCOBn(0x09);
        
        FTFA->FSTAT |= FTFA_FSTAT_CCIF(1);
        

        while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)!= FTFA_FSTAT_CCIF_MASK)
        {
        }
    }
}
void Wrie2Flash(uint32 u32Adress, uint32 u32Data)
{
    uint8 u8FCCOB1;
    uint8 u8FCCOB2;
    uint8 u8FCCOB3;
    
    uint8 u8FCCOB4;
    uint8 u8FCCOB5;
    uint8 u8FCCOB6;
    uint8 u8FCCOB7;
    
    u8FCCOB1 = (u32Adress & FCCOB1_MASK) >> FCCOB1_SHIFT;
    u8FCCOB2 = (u32Adress & FCCOB2_MASK) >> FCCOB2_SHIFT;
    u8FCCOB3 = u32Adress & FCCOB3_MASK;
    
    u8FCCOB7 = ( u32Data & FCCOB7_MASK) >> FCCOB7_SHIFT;
    u8FCCOB6 = (u32Data & FCCOB6_MASK) >> FCCOB6_SHIFT;
    u8FCCOB5 = (u32Data & FCCOB5_MASK) >> FCCOB5_SHIFT;
    u8FCCOB4 = u32Data & FCCOB4_MASK;
    
    if( u32Adress <= MAX_FLASH )
    {
        while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) != FTFA_FSTAT_CCIF_MASK)
        {
        }
        /* Adress */
        FTFA->FCCOB3 = u8FCCOB3;
        FTFA->FCCOB2 = u8FCCOB2;
        FTFA->FCCOB1 = u8FCCOB1;
        /* Command */
        FTFA->FCCOB0 = FTFA_FCCOB0_CCOBn(0x06);
        
        /* Data */
        FTFA->FCCOB4 = u8FCCOB4;
        FTFA->FCCOB5 = u8FCCOB5;
        FTFA->FCCOB6 = u8FCCOB6;
        FTFA->FCCOB7 = u8FCCOB7;
        
        FTFA->FSTAT |= FTFA_FSTAT_CCIF(1);
        while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)!= FTFA_FSTAT_CCIF_MASK)
        {
        }

    }
}