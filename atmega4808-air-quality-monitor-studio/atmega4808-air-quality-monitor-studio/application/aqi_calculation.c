/******************************************************************************
 *  include files
 *****************************************************************************/
#include "aqi_calculation.h"
#include "parameters.h"
#include "nvmctrl_basic.h"

/******************************************************************************
 *  macro definitions
 *****************************************************************************/
#define NO_OF_LAST_PM_READINGS (12)

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static uint16_t pmLast12hValues[NO_OF_LAST_PM_READINGS] = {0}; //by default 0 if value is not available


/*******************************************************************************
 * @Name
 *    uint16_t CalculateAveragePM(void)
 * @Param
 *    none
 * @Returns
 *    average pm
 * @Description
 *    It calculates average pm according to method provided by AirNow
 *******************************************************************************/
uint16_t CalculateAveragePM (void)
{
    int8_t arrayIndex = 0;
	uint16_t high = 0;
	uint16_t low = 0;
	uint16_t range = 0;
	uint8_t scaledRate = 0;
	uint8_t weightFactor = 0;
	float powerFactorArray[12] = {0};
	float sumOfPowerFactors = 0.0;
	float pfIntoPm = 0.0;

	
	for (arrayIndex = 10; arrayIndex >= 0; arrayIndex--) //right shift elements of array
	{
		pmLast12hValues[arrayIndex + 1] = pmLast12hValues[arrayIndex];
	}
	pmLast12hValues[0] = (uint16_t)*parameterTable[PM2_5].Parameter; // Saves PM value at 0th position
	
	 high = pmLast12hValues[0];
	 low = pmLast12hValues[0];
	
		
	for (arrayIndex = 1; arrayIndex < 12; arrayIndex++) // find out highest and lowest value
	{
		if (pmLast12hValues[arrayIndex] > high)
		{
			high = pmLast12hValues[arrayIndex];
		}
          
        if (pmLast12hValues[arrayIndex] < low)
        {
	        low = pmLast12hValues[arrayIndex];
        }

	}
	
	
	range = high - low ; // calculate and return range
	
	scaledRate = ((float) range / high)*10; //to keep calculation results in int instead of float, scaled rate is multiplied by 10

	weightFactor = 10 - scaledRate;

	if (weightFactor < 5)
	{
		weightFactor = 5;
	}


	 powerFactorArray[0] = 1;
	 powerFactorArray[1] = weightFactor / 10.0;

	 for (arrayIndex = 2; arrayIndex < 12; arrayIndex++)    //calculate and store power factor
	 {
		 powerFactorArray[arrayIndex] = powerFactorArray[arrayIndex - 1] * powerFactorArray[1];
	 }

	 for (arrayIndex = 0; arrayIndex < 12; arrayIndex++)   //add all the power factor
	 {
		 if (pmLast12hValues[arrayIndex] != 0)
		 {
			 sumOfPowerFactors += powerFactorArray[arrayIndex];
		 }

	 }

	 for (arrayIndex = 0; arrayIndex < 12; arrayIndex++)  //multiply each PM and corresponding Power factor, and add them 
	 {
		 pfIntoPm += pmLast12hValues[arrayIndex] * powerFactorArray[arrayIndex];
	 }


	 return (uint16_t) (pfIntoPm / sumOfPowerFactors);
}

/*******************************************************************************
 * @Name
 *    uint16_t CalculateAQI(uint16_t averagePm)
 * @Param
 *    average pm
 * @Returns
 *    Air Quality Index (AQI)
 * @Description
 *    Implementation of AQI formula 
 *******************************************************************************/
uint16_t CalculateAQI (uint16_t averagePm)
{
    uint16_t aqi = 0;
    if (averagePm > 250)
    {
        aqi = 300 + (averagePm - 250)*100 / 100;
    }
    else if (averagePm > 150)
    {
        aqi = 200 + (averagePm - 150)*100 / 100;
    }
    else if (averagePm > 55)
    {
        aqi = 150 + (averagePm - 55)*50 / 100;
    }
    else if (averagePm > 35)
    {
        aqi = 100 + (averagePm - 35)*50 / 20;
    }
    else if (averagePm > 12)
    {
        aqi = 50 + (averagePm - 12)*50 / 23;
    }
    else
    {
        aqi = averagePm * 50 / 12;
    }

    return aqi;
}

