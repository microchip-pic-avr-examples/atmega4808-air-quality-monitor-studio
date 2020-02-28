#ifndef STORE_H
#define	STORE_H

#include <stdint.h>
#include "timeout.h"

#define STORE_TASK_INTERVAL (100L)

#define EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS (2)

#define LENGTH_OF_EEPROM_ADDRESS (4)
#define LAST_EEPROM_ADDRESS (0x0003FF00UL)              //last addres of eeprom - 255

absolutetime_t STORE_Task(void *payload);





#endif	/* STORE_H */