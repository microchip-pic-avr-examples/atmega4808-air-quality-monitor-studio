#ifndef PARAMETER_H
#define	PARAMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>


#define RESET_COUNTER (0)
#define ON_BOARD_TEMP_SENSOR
#define SYNC_BYTE_ADDRESS (10)



typedef enum {
INIT_PARAMETER,

AQI,
TEMPERATURE,
HUMIDITY,
PM2_5,
TVOC,
CO2,

LAST_PARAMETER

}eParameterIndex;

typedef struct {
  eParameterIndex  ParameterIndex;
  int16_t* Parameter;
  const char*  ParameterName;
  const char* JsonFormat;
  const char* unit;
}Paramete_table_t;

typedef enum
{
   TASK_AQM,
   PARAMETER_AQM,
   INFO_AQM,
   DEBUG_AQM
}message_type_t;

extern volatile uint8_t wakeupSource;      // to store wake up source
extern volatile uint8_t runningTask;   // to store task status
extern Paramete_table_t parameterTable[];


void ParameterInit(void);
void AqmPrinter(uint8_t messageType, char* message, ...);
void ResetWriteAddress (void);

#ifdef	__cplusplus
}
#endif

#endif	/* PARAMETER_H */
