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

#define introductionFile "../Code/introduction.txt"
FILE* currentFile;

#define easyText "\nEasy(1): 2x chance of rare gems, 200 energy, no impassable cells\n"
#define easyRarityFactor 2
#define easyImpassables 0
#define normalText "\nNormal(2): 1x chance of rare gems, 150 energy points, 2 impassable cells\n"
#define normalRarityFactor 1
#define normalImpassables 2
#define hardText "\nHard(3): 0.75x chance of rare gems, 100 energy points, 4 impassable cells\n"
#define hardRarityFactor 0.75
#define hardImpassables 4

#define mapSize 10

typedef struct mineableItem{
    float value, rarity, weight;
}mineableItem;

typedef struct cell{
    int mineable;
    mineableItem item;
    char name[15], icon;
} cell;

cell uninitialised = {.icon='U'};
cell shopCell = {.name="Shop", .icon='$', .mineable=0,};
cell impassableCell = {.name="Impassable",.icon='X', .mineable=0,};
cell ironOre = {.name="Iron Ore", .icon='I', .mineable=1, .item={.value=1, .weight=1, .rarity = 0.1}};
cell goldOre = {.name="Gold Ore", .icon='G', .mineable=1, .item={.value=2, .weight=2, .rarity = 0.05}};
cell diamond = {.name="Diamond", .icon='D', .mineable=1, .item={.value=5, .weight=5, .rarity = 0.01}};
cell ground = {.name="Ground", .icon=' ', .mineable=0};

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

void gameSetup(cell (*mapToInit)[mapSize]);