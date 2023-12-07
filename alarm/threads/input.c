#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdio.h>
#include <string.h>
#include "../modules/misc.h"
#include "../modules/lcd.h"
#include "input.h"

int unl = 0;
char expectedSequence[] = {'R', 'G', 'B', 'Y'};
static pthread_t threadPID;

// helpers
char readButtonInput();
int checkSequence(const char *enteredSequence, const char *expectedSequence, int sequenceLength);
void generateRandomSequence(char expectedSequence[]);

static void* runMod(void* unused){
    // TODO:config all pins

    const int sequenceLength = 4;  
    char enteredSequence[sequenceLength];
    // char code[4] = "";
    
    generateRandomSequence(expectedSequence);

    // // put code in variable code
    // strcat(code,expectedSequence[0]);
    // strcat(code,expectedSequence[1]);
    // strcat(code,expectedSequence[2]);
    // strcat(code,expectedSequence[3]);
    // Print the generated random sequence
    printf("Expected Sequence: %s\n",expectedSequence);

    while (1) {
        
        char userInput = readButtonInput();

        // Handle the user input
        switch (userInput) {
            case 'R':
            case 'G':
            case 'B':
            case 'Y':
                // Add the button press to the entered sequence
                for (int i = 0; i < sequenceLength - 1; i++) {
                    enteredSequence[i] = enteredSequence[i + 1];
                }
                enteredSequence[sequenceLength - 1] = userInput;

                // Check if the entered sequence matches the expected sequence
                if (checkSequence(enteredSequence, expectedSequence, sequenceLength)) {
                    printf("Correct sequence matched!\n");
                    unl = 1;
                }

                sleepForMs(500);
                break;
            default:
                // Handle the default case (e.g., no button pressed)
                break;
        }
    }

    return NULL;
}

void inputInit(){
    // create thread
    int retVal = pthread_create(&threadPID, NULL, runMod, NULL);

    // error checking
    if (retVal != 0){
        // error code
        perror("input thread error");
        exit(1);
    }
}


void inputCancel(){
    pthread_cancel(threadPID);
}

void inputShutDown(){
    // REMEBER TO REMOVE CACNEL
    // inputCancel();

    pthread_join(threadPID, NULL);
}

int unlocked(){
    return unl;
}

char* getCode(){
    return expectedSequence;
}

char readButtonInput() {
    int red = readFromFileToScreen("/sys/class/gpio/gpio3/value");
    int blue = readFromFileToScreen("/sys/class/gpio/gpio5/value");
    int green = readFromFileToScreen("/sys/class/gpio/gpio31/value");
    int yellow = readFromFileToScreen("/sys/class/gpio/gpio30/value");

    if (red) {
        return 'R';
    } else if (green) {
        return 'G';
    } else if (blue) {
        return 'B';
    } else if (yellow) {
        return 'Y';
    }

    // Return a default value (you may want to handle this case based on your application logic)
    return 'N';
}

// Function to generate a random 4-code sequence
void generateRandomSequence(char expectedSequence[]) {
    const char colors[] = {'R', 'G', 'B', 'Y'};
    const int sequenceLength = 4;

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    for (int i = 0; i < sequenceLength; i++) {
        // Generate a random index within the colors array
        int randomIndex = rand() % sizeof(colors);

        // Assign a random color from the colors array to the expectedSequence
        expectedSequence[i] = colors[randomIndex];
    }
}

int checkSequence(const char *enteredSequence, const char *expectedSequence, int sequenceLength) {
    for (int i = 0; i < sequenceLength; i++) {
        if (enteredSequence[i] != expectedSequence[i]) {
            return 0;  // Mismatch
        }
    }
    return 1;  // Match
}