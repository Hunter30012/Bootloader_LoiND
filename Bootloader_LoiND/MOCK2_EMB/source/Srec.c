#include "MKL46Z4.h"
#include "My_Lib.h"
#include "Srec.h"
#include "string.h"
#include "Queue.h"
#include "Flash.h"
/*****************************************
*Varibles
*****************************************/

/****************************************
*code
****************************************/
static void SetMSP(uint32_t value)
{
   __asm("MSR MSP, R0");
}
static void SetPC(uint32 value)
{
    __asm("BLX R0");
}
void LoadApp()
{
    uint32 *u32MSP;
    uint32 *u32PC;
    u32PC = (uint32 *)0xA004;
    u32MSP = (uint32*)0XA000;
    SetMSP(*u32MSP);
    SetPC(*u32PC);
}

void tostring(char *str, uint32 u32num)
{
    uint8 i;
    uint8 u8rem;
    uint8 u8len;
    uint32 u32n;
    u32n = u32num;
    u8len = 0;
    while (u32n != 0)
    {
        u8len++;
        u32n /= 10;
    }
    for (i = 0; i < u8len; i++)
    {
        u8rem = u32num % 10;
        u32num = u32num / 10;
        str[u8len - (i + 1)] = u8rem + '0';
    }
    str[u8len] = '\0';
}

static uint32 string2dec(char *s)
{
    uint8 i;
    uint32 u32return;
    u32return = 0;
    for(i = 0; i< strlen(s); i++)
    {
        if( (s[i] >= 'A') && (s[i] <= 'F') )
        {
             u32return += (s[i] - 'A' + 10)  << (4*(strlen(s) - 1 -i));
        }
        else if ((s[i] >= '0') && (s[i] <= '9'))
        {
             u32return += (s[i] - '0')  << (4*(strlen(s) - 1 -i));   
        }
          
    }
    return u32return;
}

uint8 check_sum(uint8 *u8s, uint8 u8len)
{
     uint8 u8check;
     uint32 u32sum;
     uint8 i;
     u32sum = 0;
     u8check = 1;
     i = 2;
     while( i < (u8len - 1) )
     {
         if( (u8s[i] >= 'A') && (u8s[i] <= 'F') )
         {
            u32sum += (u8s[i] - 'A' + 10) * 16;
         }
         else if ((u8s[i] >= '0') && (u8s[i] <= '9'))
         {
            u32sum += (u8s[i] - '0') * 16;
         }
         if( (u8s[i+1] >= 'A') && (u8s[i+1] <= 'F') )
         {
            u32sum += (u8s[i+1] - 'A' + 10);
         }
         else if ((u8s[i+1] >= '0') && (u8s[i+1] <= '9'))
         {
            u32sum += (u8s[i+1] - '0');
         }
         
         i += 2;
     }
     if( 0xFF != (u32sum & 0xFF))
     {
          u8check = 0;
     }
     return u8check;
}

uint8 check_bytecount(uint8 *u8s, uint8 u8len)  /*check so byte trong truong bytecount*/
{
     uint16 u16sum;
     char str[3];
     uint8 u8temp;
     uint8 u8check;
     u8temp = 0;
     u16sum = 0;
     str[0] = u8s[2];
     str[1] = u8s[3];
     str[2] = '\0';
     u16sum = string2dec(str);
     u8temp = u8len - 4;
     if( (u16sum == (u8temp/2)) && (u8temp % 2 == 0))
     {
          u8check = 1;
     }
     else
     {
          u8check = 0;
     }
     return u8check;
}

uint8 check_line(uint8 *u8s, uint8 u8len) /*1 la dung, 0 la sai <- check cac ki tu trong dong*/
{
     uint8 i;
     uint8 u8flag;
     uint8 u8check;
     u8check = 1;
     u8flag=1;
     if( u8len%2 != 0) /*check so ki tu la so chan*/
     {
          u8check = 0;
     }
     if( u8s[0] != 'S')
     {
          u8check = 0;
     }
     i = 1;
     while((i < u8len-1) && (1 == u8flag))
     {
          if( ((u8s[i] >= 'A') && (u8s[i] <= 'F')) || ((u8s[i] >= '0') && (u8s[i] <= '9')) )
          {
               u8flag = 1;
          }
          else
          {
               u8flag = 0;
               u8check = 0;
          }
          i++;
     }
     return u8check;
}

void Process_srec(buffer_data_t  *read_adress)
{
    uint8 i;
    uint8 j;
    char adr[10];
    char data[10];
    uint32 u32Adress;
    uint32 u32Data; 
    u32Adress = 0;
    u32Data = 0;
    if( read_adress->buf[1] == '1' )
    {
        /* S1 */
        /* process adress: write to Flash*/
        for(i = 4; i < 8; i++)
        {
            adr[i - 4] = read_adress->buf[i];
        }
        adr[4] = '\0';
        u32Adress = string2dec(adr);
        /****************************/
        i = 8;
        j = 0;
        while( i < read_adress->u8length - 2)
        {
            data[j] = read_adress->buf[i];
            j++;
            i++;
            if( (i % 8 == 0)  ) /* get 4 byte */
            {
                if( u32Adress >= 0xA000 )
                {
                    data[j] = '\0';
                    u32Data = string2dec(data);
                    Wrie2Flash(u32Adress, u32Data);
                    u32Adress += 4;
                }
                j = 0;
            }
            if( (i % 8 != 0) && ( i == read_adress->u8length - 2) )
            {
                if( u32Adress >= 0xA000 )
                {
                    data[j] = '\0';
                    u32Data = string2dec(data);
                    if( j/2 == 1)   /* data 1byte */
                    {
                        u32Data = (u32Data << 24) | (0xFFFFFF);
                    }
                    else if( j/2 == 2)  /*data 2 byte*/
                    {
                        u32Data = (u32Data << 16) | (0xFFFF);
                    }
                    else /*data 3 byte */
                    {
                        u32Data = (u32Data << 8) | (0xFF);
                    }
                    Wrie2Flash(u32Adress, u32Data);
                }
            }
        }
        u32Adress = 0;
        u32Data = 0;
    }
    else if( read_adress->buf[1] == '2' ) /****** S2 ********/
    {
        /* process adress: write to Flash*/
        for(i = 4; i < 10; i++)
        {
            adr[i - 4] = read_adress->buf[i];
        }
        adr[4] = '\0';
        u32Adress = string2dec(adr);
        /****************************/
        i = 10;
        j = 0;
        while( i < read_adress->u8length - 2)
        {
            data[j] = read_adress->buf[i];
            j++;
            i++;
            if( ( (i - 2) % 8 == 0)  ) /* get 4 byte*/
            {
                if( u32Adress >= 0xA000 )
                {
                    data[j] = '\0';
                    u32Data = string2dec(data);
                    Wrie2Flash(u32Adress, u32Data);
                    u32Adress += 4;
                }
                j = 0;
            }
            if( ( (i - 2) % 8 != 0) && ( i == read_adress->u8length - 2) )
            {
                if( u32Adress >= 0xA000 )
                {
                    data[j] = '\0';
                    u32Data = string2dec(data);
                    if( j/2 == 1)   /* data 1byte */
                    {
                        u32Data = (u32Data << 24) | (0xFFFFFF);
                    }
                    else if( j/2 == 2)  /*data 2 byte*/
                    {
                        u32Data = (u32Data << 16) | (0xFFFF);
                    }
                    else /*data 3 byte */
                    {
                        u32Data = (u32Data << 8) | (0xFF);
                    }
                    Wrie2Flash(u32Adress, u32Data);
                }
            }
        }
        u32Adress = 0;
        u32Data = 0;
    }
}
uint8 Check_Timeout()
{
    static volatile uint32 u32count = 0;
    volatile uint8 u8return;
    u8return = 0;
    u32count++;
    if( u32count == TIME_OUT )
    {
        u32count = 0;
        u8return = 1;
    }
    return u8return;
}
void Backup_firmware()
{
    uint32 *u32adress_read;
    uint32 u32adress_write;
    uint32 u32data;
    uint32 i;
    Erase_Flash(0XC000);
    Erase_Flash(0xC400);
    u32adress_read = (uint32 *)0xA000;
    u32adress_write = 0xC000;
    i = 0;
    while( i < 512) /* coppy 2 sector */
    {
        u32data = *u32adress_read;
        u32data = (u32data << 24) | ((u32data & 0xff00) << 8) | ((u32data & 0xff0000) >> 8) | ((u32data & 0xff000000) >> 24);
        Wrie2Flash(u32adress_write, u32data);
        u32adress_read++;
        u32adress_write+= 4;
        i++;
    }
}
void Restore_firmware()
{
    uint32 *u32adress_read;
    uint32 u32adress_write;
    uint32 u32data;
    uint32 i;
    Erase_Flash(0XA000);
    Erase_Flash(0xA400);
    u32adress_read = (uint32 *)0xC000;
    u32adress_write = 0xA000;
    i = 0;
    while( i < 512) /* coppy 2 sector */
    {
        u32data = *u32adress_read;
        u32data = (u32data << 24) | ((u32data & 0xff00) << 8) | ((u32data & 0xff0000) >> 8) | ((u32data & 0xff000000) >> 24);
        Wrie2Flash(u32adress_write, u32data);
        u32adress_read++;
        u32adress_write+= 4;
        i++;
    }
}
