//
// Created by Alek.Walewski on 06/05/2025.
//

#include "variables.h"

mineableItem none = {};
cell uninitialised = {.icon='U'};
cell shopCell = {.icon='$', .mineable=0};
cell impassableCell = {.icon='X', .mineable=0};
cell ironOre = {.icon='I', .mineable=1, .item={.name="Iron Ore", .value=1, .weight=1, .rarity = 0.1}};
cell goldOre = {.icon='G', .mineable=1, .item={.name="Gold Ore", .value=2, .weight=2, .rarity = 0.05}};
cell diamond = {.icon='D', .mineable=1, .item={.name="Diamond", .value=5, .weight=5, .rarity = 0.01}};
cell ground = {.icon=' ', .mineable=0};

upgrade bag1 = {.name="Bag upgrade 1", .value=20, .cost=10, .available=1};

// Delay function to allow for time delay between events
void delay(int delayTime){
    // Convert input in seconds to milliseconds
    int milliSeconds = 1000 * delayTime;
    // Store current time
    clock_t startTime = clock();
    // Loop until wanted time has passed
    while (clock() < startTime + milliSeconds);
}

// Function to wait for key press and return the ascii value of the key pressed
int getKey(){
    while(1){
        if(kbhit()){ // check if keyboard hit
            return getch();
        }
    }
}