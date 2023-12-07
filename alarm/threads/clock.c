#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdio.h>
#include <string.h>
#include "../modules/lcd.h"
#include "clock.h"
#include "alarm.h"

static pthread_t threadPID;
char alarmT[6];

static void* runMod(void* unused){

    // Set the timezone to PST (Pacific Standard Time)
    setenv("TZ", "PST8PDT", 1);
    tzset(); // Update timezone information
    initializeLCD(); // init lcd
    char prevString[16] = "CLOCK: 0:00";

    while(1){
        time_t t = time(NULL);
        if (t == (time_t)(-1)) {
            return NULL;
        }
        // printf("Current time is %ld\n", t);
        struct tm date = *localtime(&t);
        
        // create a string with time
        char timeString[6]; // "HH:MM\0"
        snprintf(timeString, sizeof(timeString), "%02d:%02d", date.tm_hour, date.tm_min);

        // check if real time equal to alarm
        // signal alarm thread that plays sound and waits for input TODO:
        if(strcmp(alarmT, timeString) == 0){
            // alarm time
            alarmStart();
        }

        // Existing string
        char existingString[16] = "CLOCK: ";
        

        // Concatenate the existing string with the time string
        strcat(existingString, timeString);

        if(!(strcmp(existingString, prevString) == 0)){
            // clear before writing
            clearScreen();
            writeMessage(existingString);
            strcpy(prevString,existingString);


            writeMessage("                            ");

            // Existing string
            char ala[16] = "ALARM: ";
            // Concatenate the existing string with the time string
            strcat(ala, alarmT);
            writeMessage(ala); // put alarm variable in
        }
    }

    return NULL;
}

void clockInit(char a[6]){

    // set alarm var
    strcpy(alarmT,a);

    // create thread
    int retVal = pthread_create(&threadPID, NULL, runMod, NULL);

    // error checking
    if (retVal != 0){
        // error code
        perror("clock thread error");
        exit(1);
    }
}


void clockCancel(){
    pthread_cancel(threadPID);
}

void clockShutDown(){
    // REMEBER TO REMOVE CACNEL
    // clockCancel();

    pthread_join(threadPID, NULL);
}