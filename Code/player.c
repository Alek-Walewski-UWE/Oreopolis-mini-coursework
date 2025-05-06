//
// Created by Alek.Walewski on 06/05/2025.
//

#include "player.h"

// Function to output player stats
void showPlayerStats(player *playerStats){
    printf("\nPlayer %s stats:\n Energy: %d/%d\n Money: %d\n Weight in bag: %d/%d", playerStats->name, playerStats->energy, playerStats->defaultEnergy, playerStats->money, playerStats->weightInBag, playerStats->maximumWeight);
}

// Function to move the player around the grid
void moveCharacter(int direction, player *characterToMove, cell (*mapToCheckMovement)[MAPSIZE]){
    int newX=0, newY=0;
    switch (direction) {
        case 1:
            // Move player right
            newX = characterToMove->xCoordinate + 1;
            newY = characterToMove->yCoordinate;
            break;
        case 2:
            // Move player left
            newX = characterToMove->xCoordinate - 1;
            newY = characterToMove->yCoordinate;
            break;
        case 3:
            // Move player up
            newX = characterToMove->xCoordinate;
            newY = characterToMove->yCoordinate - 1;
            break;
        case 4:
            // Move player down
            newX = characterToMove->xCoordinate;
            newY = characterToMove->yCoordinate + 1;
            break;
        default:
            return;
    }

    // Check new coordinates won't place player out of map
    if (!((newX >= MAPSIZE) | (newX < 0) | (newY >= MAPSIZE) | (newY < 0))){
        // Check new coordinates won't place player at impassable cell
        if (mapToCheckMovement[newX][newY].icon != 'X'){
            characterToMove->xCoordinate = newX;
            characterToMove->yCoordinate = newY;

            characterToMove->energy -= 1 + (characterToMove->weightInBag * DEPLETIONFACTOR);
            return;
        }
    }
    printf("\a");
}

// Function to allow player to mine ores/gems
void mineItem(cell (*mapToEdit)[MAPSIZE], player *playerMining){
    int x = playerMining->xCoordinate, y = playerMining->yCoordinate;
    // Check if picking up item is valid or not
    if (((mapToEdit[x][y].item.weight + playerMining->weightInBag) > playerMining->maximumWeight) | (playerMining->itemsInInventory == 10)){
        printf("\a\nCannot pickup item! Remove or sell items in your inventory to pick it up.");
        delay(1.5);
        return;
    }else{
        // Add item to inventory
        playerMining->inventory[playerMining->itemsInInventory] = mapToEdit[x][y].item;
        playerMining->itemsInInventory++;
        playerMining->weightInBag += mapToEdit[x][y].item.weight;

        printf("\n%s mined successfully!", mapToEdit[x][y].item.name);
        // Set item in map to ground
        mapToEdit[x][y] = ground;
        delay(1);
    }
}