#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h> 
#include "modules/lcd.h"
#include "threads/clock.h"
#include "threads/alarm.h"
#include "threads/input.h"

static char alarmT[6] = "02:22"; // "HH:MM\0"

int main()
{
    // run internet to target, then synctime
    clockInit(alarmT);
    alarmInit();
    inputInit();
    
    clockShutDown();
    alarmShutDown();
    inputShutDown();
    
    return 0;
}