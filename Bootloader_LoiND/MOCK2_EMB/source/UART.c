#include "MKL46Z4.h"
#include "My_Lib.h"
#include "string.h"
#include "UART.h"
/*****************************************
*Varibles
*****************************************/


/****************************************
*code
****************************************/
void InitUART0(uint32 u32Baudrate)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* enable clock for port A */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; /* enable clock for UART0 */
    MCG->C2 |= MCG_C2_RANGE(1); /* khoang tan so thach anh la 8MHz */
    MCG->C2 |= MCG_C2_EREFS(1); /* chon thach anh thay vi clock ngoai tu chan chip */
    MCG->C5 |= MCG_C5_PRDIV0(3); /* chon bo chia 4 cho PLL */
    MCG->C6 |= MCG_C6_VDIV0(0);  /* mul 24 */
    MCG->C5 |= MCG_C5_PLLCLKEN0_MASK;  /* enable MCGPLLCLK */
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    
    PORTA->PCR[1] |= PORT_PCR_MUX(2);
    PORTA->PCR[2] |= PORT_PCR_MUX(2);
    SetBaudrate(u32Baudrate,24000000);
}

void Receive_string(char *s)
{
    uint32_t i;
    i = 0;
    while( (UART0->S1 & UART0_S1_RDRF_MASK) == UART0_S1_RDRF_MASK)
    {
        s[i] = UART0->D;
        while( s[i] != 13) /* Enter */
        {
            while( (UART0->S1 & UART0_S1_RDRF_MASK) == UART0_S1_RDRF_MASK)
            {
                i++;
                s[i] = UART0->D;;
            }
        }
        s[i] = '\0';
    }
}
char Receive_char()
{
    char s;
    while( (UART0->S1 & UART0_S1_RDRF_MASK) == UART0_S1_RDRF_MASK)
    {
        s = UART0->D;
    }
    return s;
}

void Print_char(char s)
{
    while ( (UART0->S1 & UART0_S1_TDRE_MASK) != UART0_S1_TDRE_MASK)
    {
    }
    UART0->D = s;
}
void Print_string(char *s)
{
    uint32 i;
    uint32 u32length;
    u32length = strlen(s);
    for( i = 0; i < u32length; i++)
    {
        UART0->D = s[i];
        while ( (UART0->S1 & UART0_S1_TDRE_MASK) != UART0_S1_TDRE_MASK)
        {
        }
    }
}
void Print_string_array(uint8 *s, uint8 u8len)
{
    uint32 i;
    for( i = 0; i < u8len; i++)
    {
        UART0->D = s[i];
        while ( (UART0->S1 & UART0_S1_TDRE_MASK) != UART0_S1_TDRE_MASK)
        {
        }
    }
}

static void SetBaudrate(uint32 u32baudrate, uint32 u32clockFreq)
{
    uint8 i;
    uint32 u32temp;
    uint8  u8OSR;
    
    uint16 u16SBR;
    uint16 u16SBR_2;
    uint16 u16error;
    uint16 u16error_2;
    
    uint8  u8OSR_pre;
    uint16 u16SBR_pre;
    uint16 u16error_pre;
    /********* create pre value ***********************/
    u8OSR = 3;
    u32temp = u32clockFreq / u32baudrate;
    u16SBR = u32temp/(u8OSR + 1);
    u16SBR_2 = u16SBR + 1;
    u16error = u32clockFreq/( (u8OSR + 1)*u16SBR ) - u32baudrate;
    u16error_2 = u32baudrate - u32clockFreq/( (u8OSR + 1)*u16SBR );
    if( u16error_2 <= u16error )
    {
        u16SBR_pre = u16SBR_2;
        u16error_pre = u16error_2;
        u8OSR_pre = u8OSR;
    }
    else
    {
        u16SBR_pre = u16SBR;
        u16error_pre = u16error;
        u8OSR_pre = u8OSR;
    }
    /*********************************************/
    for( i = 0; i< 28; i++)
    {
        u8OSR = i + 4;
        u32temp = u32clockFreq / u32baudrate;
        u16SBR = u32temp/(u8OSR + 1);
        u16SBR_2 = u16SBR + 1;
        u16error = u32clockFreq/( (u8OSR + 1)*u16SBR ) - u32baudrate;
        u16error_2 = u32baudrate - u32clockFreq/( (u8OSR + 1)*u16SBR_2 );
        if( u16error_2 <= u16error )
        {
            u16SBR = u16SBR_2;
            u16error = u16error_2;
        }
        if( u16error <= u16error_pre )
        {
            u16SBR_pre = u16SBR;
            u16error_pre = u16error;
            u8OSR_pre = u8OSR;
        }
    }
    
    UART0->C2 &= ~UART0_C2_RE_MASK;
    UART0->C2 &= ~UART0_C2_TE_MASK;
    UART0->C4 = u8OSR_pre;
    if( u16SBR > MAX_BDL)
    {
        UART0->BDL = u16SBR & UART0_BDL_SBR_MASK;
        UART0->BDH = (u16SBR_pre & SBR_BDH_MASK) >> (SBR_BDH_SHIFT);
    }
    else
    {
        UART0->BDL = u16SBR_pre;
    }
    
    UART0->C2 |= UART0_C2_RE_MASK;
    UART0->C2 |= UART0_C2_TE_MASK;
    
}
