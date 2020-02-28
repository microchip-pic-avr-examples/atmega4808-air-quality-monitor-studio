/******************************************************************************
 *  include files
 *****************************************************************************/
#include "store.h"
#include "sleep.h"
#include "../parameters.h"
#include "timeout.h"
#include "eeprom3_click.h"
#include "nvmctrl_basic.h"

/******************************************************************************
 *  enum
 *****************************************************************************/
typedef enum
{
    GET_WRITE_INDEX,
    WRITE_ONE_PARAMETER,
    UPDATE_WRITE_INDEX,
    WAITING
} store_task_state_t;

/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
timer_struct_t STORE_TasksTimer = {STORE_Task};

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static uint8_t parameterStoreCounter = AQI;
static unsigned long int eepromWriteIndex = 0;
static store_task_state_t state = GET_WRITE_INDEX;

/*******************************************************************************
 * @Name
 *    absolutetime_t STORE_Task(void *payload)
 * @Param
 *    payload address
 * @Returns
 *    next "time" to call this task
 * @Description
 *    Store data in external EEPROM
 *******************************************************************************/
absolutetime_t STORE_Task (void *payload)
{
    switch (state)
    {
    case GET_WRITE_INDEX:
//        AqmPrinter(DEBUG_AQM, "in GET_WRITE_INDEX store task");
        NVM_read_eeprom_block (EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS,
                             (uint8_t *) & eepromWriteIndex, LENGTH_OF_EEPROM_ADDRESS); //read(from internal eeprom) the address to which data has to be write in external eeprom  
        state = WRITE_ONE_PARAMETER;

        break;

    case WRITE_ONE_PARAMETER:
 //       AqmPrinter (DEBUG_AQM, "in WRITE_SUBTASK store task");
        if (parameterStoreCounter < LAST_PARAMETER)
        {
            EEPROM3_WriteOneByte (eepromWriteIndex, (uint8_t) (*parameterTable[parameterStoreCounter].Parameter >> 8));
            eepromWriteIndex++;
            EEPROM3_WriteOneByte (eepromWriteIndex, (uint8_t) (*parameterTable[parameterStoreCounter].Parameter));
            eepromWriteIndex++;

            parameterStoreCounter++;
        }
        else
        {
            parameterStoreCounter = AQI;
            state = UPDATE_WRITE_INDEX;
        }

        break;

    case UPDATE_WRITE_INDEX:
//        AqmPrinter(TASK_AQM, "in UPDATE_WRITE_INDEX store task");
        AqmPrinter (INFO_AQM, "write index = %lu", eepromWriteIndex);
        if (eepromWriteIndex > LAST_EEPROM_ADDRESS) // if write address is more than last address
        {
            eepromWriteIndex = 0x00000000; //reset the eeprom write address
        }
        NVM_write_eeprom_block (EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS,
                              (uint8_t *) & eepromWriteIndex, LENGTH_OF_EEPROM_ADDRESS); // update write address in internal eeprom
        state = WAITING;
        runningTask &= ~STORE_TASK_MASK_BIT;

        break;

    case WAITING:
//        AqmPrinter(DEBUG_AQM, "in WAITING_SUBTASK store task");
        if (runningTask & STORE_TASK_MASK_BIT)
        {
            state = GET_WRITE_INDEX;
            wakeupSource &= ~(STORE_TASK_TIME_ARRIVED); // clear wake up source flag
        }

        break;
    }

    return STORE_TASK_INTERVAL;

}