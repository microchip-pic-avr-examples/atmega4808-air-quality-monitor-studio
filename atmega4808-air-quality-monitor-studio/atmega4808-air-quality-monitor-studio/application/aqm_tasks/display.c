/******************************************************************************
 *  include files
 *****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "display.h"
#include "sleep.h"
#include "../parameters.h"
#include "sensors/oledb/oled.h"
#include "ccl.h"
#include "timeout.h"
#include "clock_config.h"
#include <util/delay.h>


/******************************************************************************
 *  enum
 *****************************************************************************/
typedef enum
{
    PRINT_ON_DISPLAY,
    TURN_OFF_DISPLAY,
    WAITING
} dispaly_task_state_t;

/******************************************************************************
 *  Macros definitions
 *****************************************************************************/
#define MAX_DISPLAY_PARAMETER       (7)
#define RESET_PAR_DISPALY_CNTR      (1)

/******************************************************************************
 *  extern parameters
 *****************************************************************************/
extern volatile uint8_t wakeupSource;
extern volatile uint8_t runningTask;

/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
volatile uint8_t displayTaskCounter = RESET_DISPLAY_TASK_COUNTER; // time to display single parameter
volatile uint8_t switchPressed = 0;

timer_struct_t DISPLAY_TasksTimer = {DISPLAY_Task};

/******************************************************************************
 *  local function declarations
 *****************************************************************************/
void DisplayOnOLED (void);

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static uint8_t parameterDisplayCounter = RESET_PAR_DISPALY_CNTR; // parameter to be displayed  
static dispaly_task_state_t state = PRINT_ON_DISPLAY;

/*******************************************************************************
 * @Name
 *    absolutetime_t DISPLAY_Task(void *payload)   
 * @Param
 *    payload address
 * @Returns
 *    next "time" to call this task
 * @Description
 *    Display data on OLED
 *******************************************************************************/
absolutetime_t DISPLAY_Task (void *payload)
{
    switch (state)
    {

    case PRINT_ON_DISPLAY:
        AqmPrinter (DEBUG_AQM, "in DISPLAY_SUBTASK display task");
        if (parameterDisplayCounter >= MAX_DISPLAY_PARAMETER)
        {
            parameterDisplayCounter = RESET_PAR_DISPALY_CNTR;
        }
        DisplayOnOLED (); //display data on OLED
        parameterDisplayCounter++; //next parameter            
        state = WAITING;

        break;
    case TURN_OFF_DISPLAY:
        //display task completed
        OLED_Clear ();
		AqmPrinter(INFO_AQM, "display task completed");
        runningTask &= ~(DISPLAY_TASK_MASK_BIT); // clear display task running bit
        parameterDisplayCounter = RESET_PAR_DISPALY_CNTR; // reset parameter display counter 
        state = WAITING;

        break;
    case WAITING:
//		AqmPrinter(DEBUG_AQM, "in WAITING_SUBTASK display task");

        if (runningTask & DISPLAY_TASK_MASK_BIT)
        {
            displayTaskCounter++;
            if ((displayTaskCounter >= MAX_DISPLAY_TASK_COUNTER) || (switchPressed == YES)) //if switch is pressed or time has elapsed 
            {
                if ((parameterDisplayCounter >= MAX_DISPLAY_PARAMETER) && (switchPressed == NO)) //if all parameters are displayed and switch is not pressed
                {
                    state = TURN_OFF_DISPLAY;
                }
                else
                {
                    state = PRINT_ON_DISPLAY;
                }
                displayTaskCounter = RESET_DISPLAY_TASK_COUNTER; // reset display task counter
                switchPressed = NO; // clear switch pressed flag
                wakeupSource &= ~(SWITCH_PRESS_EVENT); // clear wake up source flag
            }
        }

        break;
    }

    return DISPLAY_TASK_INTERVAL;

}

/*******************************************************************************
 * @Name
 *    void DisplayOnOLED (void)
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    Displays parameter name, parameter value and parameter unit on OLED  
 *******************************************************************************/
void DisplayOnOLED (void)
{
    char dataToBePrint[10];
	uint8_t columnNumber = 0;
    OLED_Clear ();

    //print parameter name 
    OLED_SetScale (2, 1);
    OLED_PutStringByColumn (0, 0, (char *)parameterTable[parameterDisplayCounter].ParameterName);

    //print parameter 
    sprintf (dataToBePrint, "%d", *parameterTable[parameterDisplayCounter].Parameter);
    OLED_SetScale (3, 4);
    columnNumber = 16 + 40 - (10 * strlen (dataToBePrint)); //to print in middle of screen
    OLED_PutString (columnNumber, 0, dataToBePrint);




    //print parameter unit
    OLED_SetScale (1, 1);
    columnNumber = 90 - (5 * strlen (parameterTable[parameterDisplayCounter].unit)); //to print at the end
    OLED_PutString (columnNumber, 4, (char *)parameterTable[parameterDisplayCounter].unit);
}

/*******************************************************************************
 * @Name
 *    void DisplayON(void)
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    OLED initialization and display AQM  
 *******************************************************************************/
void DisplayON (void)
{
    OLED_Init ();
    OLED_Clear ();
    OLED_SetScale (4, 4);
    OLED_PutString (10, 0, "AQM");
}

/*******************************************************************************
 * @Name
 *    ISR(CCL_CCL_vect)
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    CCL interrupt function. 
 *	  This interrupt occurs whenever there is switch press event.
 *******************************************************************************/
ISR(CCL_CCL_vect)
{
    /*
    * Insert your CCL interrupt handling code
    *
    * Interrupt flag can be cleared by writing 1 to its bit location
    * in the INTFLAGS register
    */
        
    wakeupSource |= SWITCH_PRESS_EVENT;
    runningTask |= DISPLAY_TASK_MASK_BIT;
    displayTaskCounter = MAX_DISPLAY_TASK_COUNTER;
    switchPressed = YES;
    
    CCL.INTFLAGS = CCL_INT1_bm;
}