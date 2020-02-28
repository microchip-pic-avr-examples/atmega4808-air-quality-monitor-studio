#ifndef DISPLAY_H
#define	DISPLAY_H

#include <stdint.h>
#include "timeout.h"

#define DISPLAY_TASK_INTERVAL_FIRST (1500L)
#define DISPLAY_TASK_INTERVAL (100L)

#define MAX_DISPLAY_TASK_COUNTER    (20)       //20 for approx 2 sec           
#define RESET_DISPLAY_TASK_COUNTER  (0)

absolutetime_t DISPLAY_Task(void *payload);

void DisplayON(void);



#endif	/* DISPLAY_H */