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
void initLed()
{
    /* Enable clock for PORTD, PORTE */
    SIM->SCGC5 |= (1 << 11);/* Set bit 11 enable clock portC*/
    SIM->SCGC5 |= (1 << 12);/* Set bit 12 enable clock portD*/
    SIM->SCGC5 |= (1 << 13);/* Set bit 13 enable clock portE*/
    /* Configure multiplex of PTD5 and PTE29 and PTC3 as GPIO */
    PORTD->PCR[5]  |= PORT_PCR_MUX(1);
    PORTE->PCR[29] |= PORT_PCR_MUX(1);
    PORTC->PCR[3]  |= PORT_PCR_MUX(1);
    PORTC->PCR[12]  |= PORT_PCR_MUX(1);
    
    PORTC->PCR[3]  |=3;
    PORTC->PCR[12]  |=3;
    /* Configure PTD5 and PTE29 as output, PTC3 as input  */
    GPIOD->PDDR |= (1 << 5);
    GPIOE->PDDR |= (1 << 29);
    GPIOC->PDDR &= (~(1 << 3));
    GPIOC->PDDR &= (~(1 << 12));
    /* Clear PTD5 and PTE29 */
}
void Bootloader_UART()
{
    char index_line[5];
    uint8 u8start;
    uint8 u8check_timeout;
    uint8 u8checkbyte;
    uint8 u8checksum;
    uint8 u8checkline;
    uint8 u8flag_error;
    uint32 u32count_line;
    buffer_data_t  *read_adress;
    buffer_data_t data_line1;
    uint32* u32PC;
    /****************************/
    u32PC = (uint32*)0xA004;
    MoveVTBtoRAM();
    if( *u32PC != DATA_EMPTY )
    {
        Backup_firmware();
    }
    InitUART0(1000000);
    UART0->C2 |= UART0_C2_RIE_MASK;
    NVIC_EnableIRQ(UART0_IRQn);
    /* Init variables */
    u8check_timeout = FALSE;
    u32count_line = 1;
    u8flag_error= 0;
    u8start = 0;
    /*****************/
    /* srec: 2 sectors */ /* first: comment 2 this commands */
    Erase_Flash(0xA000);
    Erase_Flash(0xA400);
    /****************************************************/
    Print_string("Hello\r\n");
    while (1)
    {
        if(CheckEmpty() == FALSE)
        {
            u8start = 1; /* start countter timeout */
            read_adress = GetReadElementAdress();
            u8checkbyte = check_bytecount( read_adress->buf, read_adress->u8length);
            u8checksum = check_sum(read_adress->buf, read_adress->u8length);
            u8checkline = check_line(read_adress->buf, read_adress->u8length);
            if( u32count_line == 2)
            {
                data_line1 = *read_adress;
            }
            if( (u8checkbyte == TRUE ) && (u8checksum == TRUE) && (u8checkline == TRUE) )
            {
                if(u32count_line != 2)
                {
                    Process_srec(read_adress);
                }
                if( ( read_adress->buf[1] == '9') || (read_adress->buf[1] == '8') ) /* end of srec */
                {
                    if( u8flag_error == FALSE)
                    {
                        Process_srec(&data_line1);
                        Print_string("\r\n");
                        Print_string("BootLoader Complete ");
                        Print_string("\r\n");
                        Print_string("Run app ");
                        LoadApp();
                    }
                }
                u32count_line++;
            }
            else if( u8checkline == FALSE )
            {
                u8flag_error= TRUE;
                tostring(index_line, u32count_line);
                Print_string("\r\n");
                Print_string("ERROR: error char at line ");
                Print_string(index_line);
                Print_string("\r\n");
                Print_string_array(read_adress->buf , read_adress->u8length);
                u32count_line++;
                u8checkline = TRUE;
            }
            else if(u8checkbyte == FALSE)
            {
                u8flag_error= TRUE;
                tostring(index_line, u32count_line);
                Print_string("\r\n");
                Print_string("ERROR: bytecount at line ");
                Print_string(index_line);
                Print_string("\r\n");
                Print_string_array(read_adress->buf, read_adress->u8length);
                u32count_line++;
                u8checkbyte = TRUE;
            }
            else if(u8checksum == FALSE)
            {
                u8flag_error= TRUE;
                tostring(index_line, u32count_line);
                Print_string("\r\n");
                Print_string("ERROR: checksum at line ");
                Print_string(index_line);
                Print_string("\r\n");
                Print_string_array(read_adress->buf ,read_adress->u8length);
                u32count_line++;
                u8checksum = TRUE;
            }
            FreeElement();
        }
        if( u8start == TRUE)
        {
            u8check_timeout = Check_Timeout(); /* neu khong nhan duoc S9 de ket thuc srec*/
        }
        if( CheckFull() == TRUE)
        {
            u8flag_error = 1;
            Print_string("\r\n");
            Print_string("ERROR: Queue is full ");
        }
        if( (UART0->S1 & UART0_S1_OR_MASK) == UART0_S1_OR_MASK )
        {
            u8flag_error = 1;
            Print_string("\r\n");
            Print_string("ERROR: Overun UART0 ");
            UART0->S1 |= UART0_S1_OR_MASK;
        }
        if( (UART0->S1 & UART0_S1_FE_MASK) == UART0_S1_FE_MASK )
        {
            u8flag_error = 1;
            Print_string("\r\n");
            Print_string("ERROR: Framing error ");
            UART0->S1 |= UART0_S1_FE_MASK;
        }
        if( (UART0->S1 & UART0_S1_NF_MASK) == UART0_S1_NF_MASK )
        {
            u8flag_error = 1;
            Print_string("\r\n");
            Print_string("ERROR: Noise detected in the received character in UART_D ");
            UART0->S1 |= UART0_S1_NF_MASK;
        }
        if( (u8check_timeout == TRUE ) || u8flag_error == TRUE ) /* gap loi la dung viec ghi Flash */
        {
            Restore_firmware();
            if( u8check_timeout == TRUE)
            {
                Print_string("\r\n");
                Print_string("Time out ");
            }
            Print_string("\r\n");
            Print_string("Run backup ");
            LoadApp();
        }
    }
}
void Run_App()
{
    uint32* u32PC;
    u32PC = (uint32*)0xA004;
    if( *u32PC == DATA_EMPTY)
    {
        Restore_firmware();
    }
    LoadApp();
}