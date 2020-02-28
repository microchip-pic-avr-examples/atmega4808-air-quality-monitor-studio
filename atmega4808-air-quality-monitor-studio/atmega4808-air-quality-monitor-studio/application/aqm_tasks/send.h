#ifndef SEND_H
#define	SEND_H

#include <stdint.h>
#include "timeout.h"

#define SEND_TASK_INTERVAL (100L)

#define EXTERNAL_EEPROM_SEND_INDEX_ADDRESS (6)


absolutetime_t SEND_Task(void *payload);





#endif	/* SEND_H */