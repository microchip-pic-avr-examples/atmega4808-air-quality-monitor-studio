#ifndef SLEEP_H
#define	SLEEP_H

#include <stdint.h>
#include "timeout.h"

#define SLEEP_TASK_INTERVAL (100L)

#define READ_TASK_MASK_BIT                   (0x01)
#define STORE_TASK_MASK_BIT                  (0x02)
#define SEND_TASK_MASK_BIT                   (0x04)
#define DISPLAY_TASK_MASK_BIT                (0x08)
#define ALL_TASK_MASK_BIT                    (0x0F)

#define READ_TASK_TIME_ARRIVED  (0x01)
#define STORE_TASK_TIME_ARRIVED   (0x02)
#define SWITCH_PRESS_EVENT     (0x04)
#define SEND_TASK_TIME_ARRIVED      (0x08)
    


#define YES (1)
#define NO (0)

absolutetime_t SLEEP_Task(void *payload); 
void PIT_initialization(void);



#endif	/* SLEEP_H */