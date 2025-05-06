//
// Created by Alek.Walewski on 06/05/2025.
//

#ifndef OREOPOLIS_MINI_COURSEWORK_PLAYER_H
#define OREOPOLIS_MINI_COURSEWORK_PLAYER_H

#include "variables.h"

void moveCharacter(int direction, player *characterToMove, cell (*mapToCheckMovement)[MAPSIZE]);
void mineItem(cell (*mapToEdit)[MAPSIZE], player *playerMining);
void showPlayerStats(player *playerStats);

#endif //OREOPOLIS_MINI_COURSEWORK_PLAYER_H