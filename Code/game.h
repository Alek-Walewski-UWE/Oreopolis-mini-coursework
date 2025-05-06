//
// Created by Alek.Walewski on 06/05/2025.
//

#ifndef OREOPOLIS_MINI_COURSEWORK_GAME_H
#define OREOPOLIS_MINI_COURSEWORK_GAME_H

#include "variables.h"

#define introductionFile "../Code/introduction.txt"
#define leaderboardFile "../Code/leaderboard.txt"
extern FILE* currentFile;

#define LEADERBOARDHEADER "Name     |Difficulty|Time Taken\n-------------------------------\n"

int gameLoop();
void gameSetup(cell (*mapToInit)[MAPSIZE], player *characterInit);
void endGame(player *endPlayer, int endState);
void displayMap(cell (*mapToDisplay)[MAPSIZE], int characterX, int characterY);
void viewInventory(player *characterPlayer);
void openShop(player *characterPlayer);
void viewLeaderboard();

#endif //OREOPOLIS_MINI_COURSEWORK_GAME_H