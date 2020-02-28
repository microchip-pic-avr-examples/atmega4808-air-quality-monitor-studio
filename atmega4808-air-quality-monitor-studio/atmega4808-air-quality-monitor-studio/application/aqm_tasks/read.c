/******************************************************************************
 *  include files
 *****************************************************************************/
#include "read.h"
#include "sleep.h"
#include "../parameters.h"
#include "../aqi_calculation.h"
#include "../../sensors/sps30/sps30.h"
#include "../../sensors/sps30/s_shdlc.h"
#include "../../sensors/sps30/s_uart.h"
#include "adc_basic.h"
#include "usart_basic.h"
#include "../../sensors/aq3.h"
#include "sensors_handling.h"
#include "../parameters.h"
#include "timeout.h"
#include "clock_config.h"
#include <util/delay.h>

/******************************************************************************
 *  enum
 *****************************************************************************/
typedef enum
{
    INIT_PM_SENSOR,
	INIT_AQ3,
    READ_PM_SENSOR,
	READ_AQ3,
	READ_TEMP_SENSOR,
	READ_RH_SENSOR,
	CALCULATE_AQI,
    WAITING
} read_task_state_t;

/******************************************************************************
 *  Macros definitions
 *****************************************************************************/
#define HUMIDITY_SENSOR_ADC_CHANNEL (7)
#define TEMPERATURE_SENSOR_ADC_CHANNEL (6)
#define VDD (3.3)


/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
timer_struct_t READ_TasksTimer = {READ_Task};

/******************************************************************************
 *  local function declarations
 *****************************************************************************/
uint16_t AqiCalculation (void);

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static read_task_state_t state = INIT_PM_SENSOR;
static float pmReading = 0;
static uint16_t co2 = 0;
static uint16_t  tvoc = 0;
static uint16_t humidityRaw = 0;
static uint16_t temperatureRaw = 0;

/*******************************************************************************
 * @Name
 *    absolutetime_t READ_Task(void *payload)
 * @Param
 *    payload address
 * @Returns
 *    next "time" to call this task
 * @Description
 *    read sensor data
 *******************************************************************************/
absolutetime_t READ_Task (void *payload)
{
    switch (state)
    {
    case INIT_PM_SENSOR:
//		AqmPrinter(DEBUG_AQM, "in INIT_SENSOR pm task");
		sensirion_uart_open ();
	    sps30_start_measurement (); //start measurement 
		state = INIT_AQ3;		
		break;

    case INIT_AQ3:
//		AqmPrinter(DEBUG_AQM, "in INIT_SENSOR aq3 task");
		ccs811_InitSensor (); //start measurement

		state = READ_RH_SENSOR;
		break;

    case READ_RH_SENSOR:
//		AqmPrinter (TASK_AQM, "in READ_SENSOR humidity task");
		humidityRaw = ADC_0_get_conversion (HUMIDITY_SENSOR_ADC_CHANNEL);
		humidityRaw >>= 3; //divide by 8

		//convert adc reading to humidity value
		humidityRaw = 12 * humidityRaw;
		humidityRaw = humidityRaw / 100 - 12;
    
		*parameterTable[HUMIDITY].Parameter = (int16_t) humidityRaw;
		AqmPrinter (PARAMETER_AQM, "humidity : %d", humidityRaw);
    
		state = READ_TEMP_SENSOR;
		break;
	
    case READ_TEMP_SENSOR:
//		AqmPrinter (TASK_AQM, "in READ_SENSOR temperature task");
#ifdef ON_BOARD_TEMP_SENSOR
		temperatureRaw = (uint16_t) SENSORS_getTempValue ();
		temperatureRaw = temperatureRaw / 100;
#else
		temperatureRaw = ADC_0_get_conversion (TEMPERATURE_SENSOR_ADC_CHANNEL);
		temperatureRaw >>= 3; //divide by 8

		//convert adc reading to humidity value
		temperatureRaw = 21 * temperatureRaw;
		temperatureRaw = temperatureRaw / 100 - 67;
#endif

		*parameterTable[TEMPERATURE].Parameter = (int16_t) temperatureRaw;
		AqmPrinter (PARAMETER_AQM, "temperature : %d", temperatureRaw);
    
		state = READ_AQ3;   
		break;
	
    case READ_AQ3:
//		AqmPrinter (TASK_AQM, "in READ_SENSOR aq3 task");
		ccs811_GetResults (&co2, &tvoc); // read the data from sensor;
    
		*parameterTable[CO2].Parameter = (int16_t) co2;
		*parameterTable[TVOC].Parameter = (int16_t) tvoc;
		AqmPrinter (PARAMETER_AQM, "CO2 : %d    TVOC:%d ", co2, tvoc);
     
		state = READ_PM_SENSOR;
		break;
				
    case READ_PM_SENSOR:
//        AqmPrinter (TASK_AQM, "in READ_SENSOR pm task");
		
		while(!(USART2.STATUS & USART_DREIF_bm));	//wait for uart to be ready
		while(!(USART1.STATUS & USART_DREIF_bm));	//wait for uart to be ready
		
        sps30_read_measurement (&pmReading);
		
		*parameterTable[PM2_5].Parameter = (int16_t) pmReading;
		AqmPrinter (PARAMETER_AQM, "pm : %d", (uint16_t) pmReading);
		
        state = CALCULATE_AQI;       		
        break;

		
     case CALCULATE_AQI:
	     *parameterTable[AQI].Parameter = (int16_t) AqiCalculation (); //calculate AQI
//	     AqmPrinter (TASK_AQM, "calculating aqi");
		 runningTask &= ~READ_TASK_MASK_BIT;
	     state = WAITING;	 		
	     break;
		 		 
    case WAITING:
//		AqmPrinter(DEBUG_AQM, "in WAITING_SUBTASK pm task");

        if (runningTask & READ_TASK_MASK_BIT)
        {
            state = READ_RH_SENSOR;
            wakeupSource &= ~(READ_TASK_TIME_ARRIVED); // clear wake up source flag
        }
        break;
		
    }
	
    return READ_TASK_INTERVAL;
}

/*******************************************************************************
 * @Name
 *    uint16_t AqiCalculation(void)
 * @Param
 *    none
 * @Returns
 *    AQI value 
 * @Description
 *    calculates AQI by calling CalculateAveragePM() and CalculateAQI(averagePm) 
 *    functions
 *******************************************************************************/
uint16_t AqiCalculation (void)
{
	uint16_t averagePm = 0;
	uint16_t aqi = 0;
    averagePm = CalculateAveragePM (); //calculate average of last 12 pm values
//	AqmPrinter (PARAMETER_AQM, "Average PM : %d", (uint16_t) averagePm);
    aqi = CalculateAQI (averagePm); // calculate AQI
	AqmPrinter (PARAMETER_AQM, "AQI : %d", (uint16_t) aqi);

    return (aqi);
}