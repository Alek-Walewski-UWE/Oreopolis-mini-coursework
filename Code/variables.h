//
// Created by Alek.Walewski on 06/05/2025.
//

#ifndef OREOPOLIS_MINI_COURSEWORK_VARIABLES_H
#define OREOPOLIS_MINI_COURSEWORK_VARIABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define MAPSIZE 12
#define MONEYTOWIN 20

#define easyText "\nEasy(1): Highest chance of rare gems, 200 energy, no impassable cells\n"
#define easyRarityFactor 3
#define easyImpassables 0
#define easyPlayerEnergy 200
#define normalText "\nNormal(2): Normal chance of rare gems, 150 energy points, 6 impassable cells\n"
#define normalRarityFactor 2
#define normalImpassables 6
#define normalPlayerEnergy 150
#define hardText "\nHard(3): Decreased chance of rare gems, 100 energy points, 10 impassable cells\n"
#define hardRarityFactor 1.5
#define hardImpassables 10
#define hardPlayerEnergy 100

#define DEPLETIONFACTOR 0.2

extern time_t startTime;

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

typedef struct{
    int energy, defaultEnergy, maximumWeight, weightInBag, money, xCoordinate, yCoordinate, itemsInInventory;
    mineableItem inventory[10];
    char name[20], selectedDifficulty[10];
} player;

typedef struct{
    char name[20];
    int value, cost, available;
} upgrade;

void delay(int delayTime);
int getKey();

extern mineableItem none;
extern cell uninitialised;
extern cell shopCell;
extern cell impassableCell;
extern cell ironOre;
extern cell goldOre;
extern cell diamond;
extern cell ground;
extern upgrade bag1;

#endif //OREOPOLIS_MINI_COURSEWORK_VARIABLES_H