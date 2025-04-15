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
#define easyPlayerEnergy 200
#define normalText "\nNormal(2): 1x chance of rare gems, 150 energy points, 2 impassable cells\n"
#define normalRarityFactor 1
#define normalImpassables 2
#define normalPlayerEnergy 150
#define hardText "\nHard(3): 0.75x chance of rare gems, 100 energy points, 4 impassable cells\n"
#define hardRarityFactor 0.75
#define hardImpassables 4
#define hardPlayerEnergy 100

#define DEPLETIONFACTOR 0.2

#define MAPSIZE 10
#define MONEYTOWIN 20

typedef struct{
    int value, weight;
    float rarity;
    char name[15];
}mineableItem;

typedef struct{
    int mineable;
    mineableItem item;
    char icon;
} cell;

mineableItem none = {};
cell uninitialised = {.icon='U'};
cell shopCell = {.icon='$', .mineable=0};
cell impassableCell = {.icon='X', .mineable=0};
cell ironOre = {.icon='I', .mineable=1, .item={.name="Iron Ore", .value=1, .weight=1, .rarity = 0.1}};
cell goldOre = {.icon='G', .mineable=1, .item={.name="Gold Ore", .value=2, .weight=2, .rarity = 0.05}};
cell diamond = {.icon='D', .mineable=1, .item={.name="Diamond", .value=5, .weight=5, .rarity = 0.01}};
cell ground = {.icon=' ', .mineable=0};

typedef struct{
    int energy, defaultEnergy, maximumWeight, weightInBag, money, xCoordinate, yCoordinate, itemsInInventory;
    mineableItem inventory[10];
    char name[20];
} player;

typedef struct{
    char name[20];
    int value, cost, available;
} upgrade;

upgrade bag1 = {.name="Bag upgrade 1", .value=200, .cost=10, .available=1};

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

// Function to output player stats
void showPlayerStats(player *playerStats){
    printf("\nPlayer %s stats:\n Energy: %d/%d\n Money: %d\n Weight in bag: %d/%d", playerStats->name, playerStats->energy, playerStats->defaultEnergy, playerStats->money, playerStats->weightInBag, playerStats->maximumWeight);
}

void gameSetup(cell (*mapToInit)[MAPSIZE], player *characterInit);
void endGame();
void displayMap(cell (*mapToDisplay)[MAPSIZE], int characterX, int characterY);
void moveCharacter(int direction, player *characterToMove, cell (*mapToCheckMovement)[MAPSIZE]);
void mineItem(cell (*mapToEdit)[MAPSIZE], player *playerMining);
void viewInventory(player *characterPlayer);
void openShop(player *characterPlayer);