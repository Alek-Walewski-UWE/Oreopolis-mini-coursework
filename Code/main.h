//
// Created by Alek.Walewski on 26/03/2025.
//

#ifndef OREOPOLIS_MINI_COURSEWORK_MAIN_H
#define OREOPOLIS_MINI_COURSEWORK_MAIN_H

#endif //OREOPOLIS_MINI_COURSEWORK_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

// Delay function to allow for time delay between events
void delay(int delayTime){
    // Convert input in seconds to milliseconds
    int milliSeconds = 1000 * delayTime;
    // Store current time
    clock_t startTime = clock();
    // Loop until wanted time has passed
    while (clock() < startTime + milliSeconds)
        ;
}

#define introductionFile "../Code/introduction.txt"
FILE* currentFile;