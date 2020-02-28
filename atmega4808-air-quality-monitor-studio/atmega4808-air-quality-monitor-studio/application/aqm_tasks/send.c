/******************************************************************************
 *  include files
 *****************************************************************************/
#include <stdio.h>
#include "send.h"
#include "store.h"
#include "sleep.h"
#include "../parameters.h"
#include "timeout.h"
#include "cloud/cloud_service.h"
#include "led.h"
#include "clock_config.h"
#include "nvmctrl_basic.h"
#include "eeprom3_click.h"
#include <util/delay.h>

/******************************************************************************
 *  enum
 *****************************************************************************/
typedef enum
{
    CHECK_CLOUD_CONNECTION,
    GET_SEND_INDEX,
    SEND_FROM_EEPROM,
    UPDATE_SEND_INDEX,
    SEND_NEW_DATA,
    WAITING
} send_task_state_t;


/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
timer_struct_t SEND_TasksTimer = {SEND_Task};


/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static send_task_state_t task = CHECK_CLOUD_CONNECTION;
static unsigned long int eepromSendIndex = 0; //to store address of data in eeprom from which data has to be send

/*******************************************************************************
 * @Name
 *    absolutetime_t SEND_Task(void *payload)  
 * @Param
 *    payload address
 * @Returns
 *    next "time" to call this task
 * @Description
 *    read from external EEPROM (EEPROM 2 click) and Send data to cloud 
 *******************************************************************************/
absolutetime_t SEND_Task (void *payload)
{
    static char json[250]; // to store data to be send to cloud
    uint8_t newLength = 0;
    uint8_t previousLength = 0;
    uint8_t parameterReadCounter = 0;
    uint8_t ParameterValueMSB = 0;
    uint8_t ParameterValueLSB = 0;
    uint16_t ParameterValue = 0;
    unsigned long int eepromWriteIndex = 0; //to store address of data in eeprom from which data has to be store
    uint16_t len = 0;

    switch (task)
    {
    case CHECK_CLOUD_CONNECTION:
//        AqmPrinter(DEBUG_AQM, "in INIT_SUBTASK send task");
        if (CLOUD_isConnected ())
        {
            task = GET_SEND_INDEX;
        }
        else
        {
//            AqmPrinter (INFO_AQM, "no cloud connection");
            task = WAITING;
            runningTask &= ~SEND_TASK_MASK_BIT;
        }

        break;

    case GET_SEND_INDEX:
 //       AqmPrinter(DEBUG_AQM, "in GET_WRITE_INDEX send task");
        NVM_read_eeprom_block (EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS, (uint8_t *) & eepromWriteIndex, LENGTH_OF_EEPROM_ADDRESS); //read(from internal eeprom) the address to which data has to be write in external eeprom 
        NVM_read_eeprom_block (EXTERNAL_EEPROM_SEND_INDEX_ADDRESS, (uint8_t *) & eepromSendIndex, LENGTH_OF_EEPROM_ADDRESS); //read(from internal eeprom) the address fromm which data has to be send in external eeprom 
        //if eeprom is available and send index is not equal to write index do below things
        if (eepromWriteIndex != eepromSendIndex)
        {
            task = SEND_FROM_EEPROM;
//            AqmPrinter (INFO_AQM, "write index = %lu", eepromWriteIndex);
//			  AqmPrinter (INFO_AQM, "send index = %lu", eepromSendIndex);
        }
        else
        {
            task = SEND_NEW_DATA;
        }

        break;

    case SEND_FROM_EEPROM:
        AqmPrinter (DEBUG_AQM, "in SEND_FROM_EEPROM_SUBTASK send task");
        previousLength = sprintf (json, "{"); //add first character of json 
        //read data from external eeprom at send address one by one and convert it  into staring
        for (parameterReadCounter = AQI; parameterReadCounter < LAST_PARAMETER; parameterReadCounter++)
        {
            ParameterValueMSB = EEPROM3_ReadOneByte (eepromSendIndex);
            eepromSendIndex++;
            ParameterValueLSB = EEPROM3_ReadOneByte (eepromSendIndex);
            eepromSendIndex++;
            ParameterValue = ParameterValueMSB << 8 | ParameterValueLSB;
            newLength = sprintf (json + previousLength, "%s:%d,",
                                 parameterTable[parameterReadCounter].JsonFormat, (int16_t) ParameterValue);
            previousLength += newLength;
        }
        previousLength--;   //to overwrite last comma
        newLength = sprintf (json + previousLength, "}"); //add last character of json 
        previousLength += newLength;
		
        //send data to cloud
        if (previousLength > 0)
        {
            LED_flashYellow ();
            CLOUD_publishData ((uint8_t*) json, previousLength);
        }

        task = UPDATE_SEND_INDEX;

        break;
    case UPDATE_SEND_INDEX:
        if (eepromSendIndex > LAST_EEPROM_ADDRESS) //if send address if more than last eeprom address
        {
            eepromSendIndex = 0x00000000; //reset the eeprom send address
        }
        //update send address in internal EEPROM
        NVM_write_eeprom_block (EXTERNAL_EEPROM_SEND_INDEX_ADDRESS, (uint8_t *) & eepromSendIndex, LENGTH_OF_EEPROM_ADDRESS);
        task = GET_SEND_INDEX;

        break;
    case SEND_NEW_DATA:
        AqmPrinter (DEBUG_AQM, "in SEND_NEW_DATA_SUBTASK send task");
        len = sprintf (json, "{%s:%d,%s:%d,%s:%d,%s:%d,%s:%d,%s:%d}",
                       parameterTable[AQI].JsonFormat, *parameterTable[AQI].Parameter,
                       parameterTable[TEMPERATURE].JsonFormat, *parameterTable[TEMPERATURE].Parameter,
                       parameterTable[HUMIDITY].JsonFormat, *parameterTable[HUMIDITY].Parameter,
                       parameterTable[PM2_5].JsonFormat, *parameterTable[PM2_5].Parameter,
                       parameterTable[TVOC].JsonFormat, *parameterTable[TVOC].Parameter,
                       parameterTable[CO2].JsonFormat, *parameterTable[CO2].Parameter);
					   
        if (len > 0)
        {
            LED_flashYellow ();
            CLOUD_publishData ((uint8_t*) json, len); //send data to cloud 

        }
        runningTask &= ~SEND_TASK_MASK_BIT; // task is completed
        task = WAITING;

        break;

    case WAITING:
//        AqmPrinter(DEBUG_AQM, "in WAITING_SUBTASK store task");
        if (runningTask & SEND_TASK_MASK_BIT)
        {
            task = CHECK_CLOUD_CONNECTION;
            wakeupSource &= ~(SEND_TASK_TIME_ARRIVED); // clear wake up source flag
        }

        break;
    }

    return SEND_TASK_INTERVAL;
}