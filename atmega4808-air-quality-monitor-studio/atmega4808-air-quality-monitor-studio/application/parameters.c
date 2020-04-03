/******************************************************************************
 *  include files
 *****************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include "parameters.h"
#include "aqm_tasks/sleep.h"
#include "nvmctrl_basic.h"
#include "debug_print.h"
#include "aqm_tasks/send.h"
#include "aqm_tasks/store.h"

/******************************************************************************
 *  macro definitions
 *****************************************************************************/
#define SYNC_BYTE (0xAA)
#define RESET_BYTE (0xFF)

#define AQM_PRINT_ON

/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
int16_t aqi = 0;
int16_t humidity = 0;
int16_t temperature = 0;
int16_t co2 = 0;
int16_t tvoc = 0;
int16_t pm2_5 = 0;
int16_t reserved = 0;

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static unsigned long int firstEepromWriteAddress = 0x00000000; //first time write address of external eeprom 
static unsigned long int firstEepromSendAddress = 0x00000000; //first time write address of external eeprom 



Paramete_table_t parameterTable[] = {

    /*	Index	           Parameter		    Name          Json Format                              unit      */
	{INIT_PARAMETER,       &reserved,            "N",            "N",                                   "N"},

    {AQI,                  &aqi,                "AQI",         "\"1\u2024"" AQI\"",                     " "},
    {TEMPERATURE,          &temperature,        "Temp",        "\"3\u2024"" T \u00B0""C\"",             "?C"}, //? for degree
    {HUMIDITY,             &humidity,           "RH",          "\"4\u2024"" RH %\"",                    "%"},
    {PM2_5,                &pm2_5,              "PM",          "\"2\u2024"" PM ug\u2215""m\u00B3\"",    "ug/m@"}, //@ for cube
    {TVOC,                 &tvoc,               "TVOC",        "\"6\u2024"" TVOC ppb\"",                "ppb"},
    {CO2,                  &co2,                "CO2",         "\"5\u2024"" CO2 ppm\"",                 "ppm"},


    {LAST_PARAMETER,       &reserved,            "N",            "N",                                   "N"},
};

/*******************************************************************************
 * @Name
 *    void ParameterInit(void)
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    Checking if first time power on or not by reading data from SYNC_BYTE_ADDRESS
 *    of internal EEPROM
 *******************************************************************************/
void ParameterInit (void)
{
    uint8_t readByte; //to store byte which is at SYNC_BYTE_ADDRESS of internal eeprom

    readByte = NVM_read_eeprom_byte (SYNC_BYTE_ADDRESS); //reading byte from internal EEPROM , address = SYNC_BYTE_ADDRESS

    if (SYNC_BYTE != readByte)
    {
        //first time power on
        NVM_write_eeprom_block (EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS, (uint8_t *) & firstEepromWriteAddress, 4); //writing first time write address (of external eeprom) in internal eeprom
        NVM_write_eeprom_block (EXTERNAL_EEPROM_SEND_INDEX_ADDRESS, (uint8_t *) & firstEepromSendAddress, 4); //writing first time send address (of external eeprom) in internal eeprom
        NVM_write_eeprom_byte (SYNC_BYTE_ADDRESS, SYNC_BYTE); //writing SYNC_BYTE in internal eeprom 
    }

}

/*******************************************************************************
 * @Name
 *    void ResetWriteAddress(void)
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    To perform first time power on by writing any other data than SYNC_BYTE 
 *    at SYNC_BYTE_ADDRESS in internal EEPROM 
 *******************************************************************************/
void ResetWriteAddress (void)
{
    NVM_write_eeprom_byte (SYNC_BYTE_ADDRESS, RESET_BYTE); //writing RESET_BYTE in internal eeprom 
}

/*******************************************************************************
 * @Name
 *    void AqmPrinter(message_type_t messageType, char* message, ...)
 * @Param
 *    print message type , print message
 * @Returns
 *    none
 * @Description
 *    To print message on terminal 
 *******************************************************************************/

void AqmPrinter (message_type_t messageType, char* message, ...)
{
#ifdef AQM_PRINT_ON


    printf (" %s ", CSI_WHITE CSI_INVERSE " AQM " CSI_NORMAL CSI_WHITE);

    if (messageType == PARAMETER_AQM)
    {
        printf (" %s ", CSI_MAGENTA " PARAMETER  " CSI_WHITE);
    }
    else if (messageType == TASK_AQM)
    {
        printf (" %s ", CSI_YELLOW " TASK       " CSI_WHITE);
    }
    else if (messageType == INFO_AQM)
    {
        printf (" %s ", CSI_GREEN " INFO       " CSI_WHITE);
    }
    else
    {
        printf (" %s ", CSI_BLUE " DEBUG      " CSI_WHITE);
    }

    va_list argptr;
    va_start (argptr, message);
    vprintf (message, argptr);
    va_end (argptr);

    printf ("\r\n");

#endif  
}