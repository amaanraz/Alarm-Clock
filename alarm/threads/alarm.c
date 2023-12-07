#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdio.h>
#include <string.h>
#include "../modules/lcd.h"
#include "input.h"
#include "alarm.h"

static pthread_t threadPID;
static pthread_cond_t wait = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void* runMod(void* unused){

    // condition to wait until message is recieved in list (signalled by input thread)
    pthread_mutex_lock(&lock);
    {
        pthread_cond_wait(&wait, &lock);
    }
    pthread_mutex_unlock(&lock);

    int combination = unlocked();

    sleep(1);
     // write sequence to screen
    char* code = getCode();
    writeMessage(code); // get code

    while(!combination){
        combination = unlocked();

        // alarm function
        
        printf("ALARM TEST\n");
        sleep(1);

        
    }

    // this just here to test
    alarmShutDown();
    alarmInit();

    return NULL;
}

void alarmInit(){
    // create thread
    int retVal = pthread_create(&threadPID, NULL, runMod, NULL);

    // error checking
    if (retVal != 0){
        // error code
        perror("alarm thread error");
        exit(1);
    }
}


void alarmCancel(){
    pthread_cancel(threadPID);
}

void alarmStart(){
    // signal
    pthread_mutex_lock(&lock);
    {
        pthread_cond_signal(&wait);
    }
    pthread_mutex_unlock(&lock);
}

void alarmShutDown(){
    // REMEBER TO REMOVE CACNEL
    alarmCancel();

    pthread_join(threadPID, NULL);
}