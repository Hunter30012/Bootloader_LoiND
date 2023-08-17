#ifndef _SREC_H
#define _SREC_H

/****************************************************
*Defination
*****************************************************/
#define DATA_EMPTY             0xFFFFFFFF
#define WRITE_ADRESS_2_FLASH   0xA000
#define TIME_OUT                2500000
/****************************************************
*Prototypes
****************************************************/
static void SetMSP(uint32_t value);
/*Set MSP*/

static void SetPC(uint32 value);
/*Set PC*/

void LoadApp();
/* Run app */

void tostring(char *str, uint32 u32num);
/*convert number to string
  u32num -> str
*/
static uint32 string2dec(char *s);
/* convert string to decimal */

uint8 check_sum(uint8 *u8s, uint8 u8len);
/* check sum 1 line srec */

uint8 check_bytecount(uint8 *u8s, uint8 u8len);
/* check byte count*/
uint8 check_line(uint8 *u8s, uint8 u8len);
/* check error char */
void Process_srec();
/* Check and wrrite srec to Flash */
uint8 Check_Timeout();
/* Check timeout */

void Backup_firmware();
/* copppy all srec from A000 to C000 */

void Restore_firmware();
/* coppy all srec from C000 to A000 */
#endif