//
// Created by Alek.Walewski on 26/03/2025.
//
#include "main.h"

int main(){
    char readChar;
    int userInput = 0, gameLoop = 1;
    cell map[MAPSIZE][MAPSIZE] = {};
    mineableItem currentItem;
    player character = {};

    mainMenu:
    system("cls");
    // Open file with introduction text
    currentFile = fopen(introductionFile, "r");
    // Loop through and print each char until end of file is reached
    while((readChar = (char)fgetc(currentFile)) != EOF){
        printf("%c", readChar);
    }
    fclose(currentFile);

    // Allow user to choose menu option
    while(userInput == 0){
        userInput = getKey();
        switch(userInput){
            case 49:
                // Call initial game setup function
                gameSetup(&map, &character);
                // Take player name input
                printf("\nPlease enter your player name: ");
                scanf("%s", &character.name);

                printf("\nGame initiated successfully, good luck!");
                delay(1);
                break;
            case 50:
                // Call function to read leaderboard

                break;
            case 51:
                return 0;
            default:
                userInput = 0;
                break;
        }
    }

    // Main game loop
    while(gameLoop==1){
        // Check if character is out of energy and run end game function if true
        if(character.energy == 0){
            //endGame();
            break;
        }

        // Output map function
        system("cls");
        displayMap(&map, character.xCoordinate, character.yCoordinate);

        // Output player stats
        printf("\nPlayer %s stats:\n Energy: %d\n Money: %d\n Weight in bag: %d/%d", character.name, character.energy, character.money, character.weightInBag, character.maximumWeight);

        // Output player options
        printf("\nUse arrow keys to move in selected direction, I for inventory");
        // Display shop or mining option only if standing on a suitable cell
        if(map[character.xCoordinate][character.yCoordinate].icon == '$'){
            printf(" or E to open the shop");
        } else if(map[character.xCoordinate][character.yCoordinate].mineable == 1){
            currentItem = map[character.xCoordinate][character.yCoordinate].item;
            printf(" or E to mine item: %s\n Value: %.0f\n Weight: %.0f", currentItem.name, currentItem.value, currentItem.weight);
        }


        // Wait valid user input
        userInput = 0;
        while(userInput == 0){
            userInput = getKey();
            switch (userInput) {
                case 105:
                    viewInventory(&character);
                    break;
                case 77:
                    // Right
                    moveCharacter(1, &character, &map);
                    break;
                case 75:
                    // Left
                    moveCharacter(2, &character, &map);
                    break;
                case 72:
                    // Up
                    moveCharacter(3, &character, &map);
                    break;
                case 80:
                    // Down
                    moveCharacter(4, &character, &map);
                    break;
                default:
                    userInput = 0;
                    break;
            }
        }
    }

    return 0;
}

// Initial game setup function to initialise game variables and structures
void gameSetup(cell (*mapToInit)[MAPSIZE], player *characterInit){
    int difficulty = 0, impassables, x, y;
    float gemRarityFactor, gemProbability;
    // Prompt user to select difficulty
    printf("Please select one of the following difficulties: %s%s%s", easyText, normalText, hardText);
    while(difficulty == 0){
        difficulty = getKey();
        switch (difficulty) {
            case 49:
                // Initiate easy difficulty variables
                gemRarityFactor = easyRarityFactor;
                impassables = easyImpassables;
                characterInit->energy = easyPlayerEnergy;
                break;
            case 50:
                // Initiate normal difficulty variables
                gemRarityFactor = normalRarityFactor;
                impassables = normalImpassables;
                characterInit->energy = normalPlayerEnergy;
                break;
            case 51:
                // Initiate hard mode difficulty
                gemRarityFactor = hardRarityFactor;
                impassables = hardImpassables;
                characterInit->energy = hardPlayerEnergy;
                break;
            default:
                difficulty = 0;
                break;
        }
    }

    // Set character variables that are the same for every difficulty
    characterInit->maximumWeight = 100;
    characterInit->weightInBag = 0;
    characterInit->money = 0;
    characterInit->xCoordinate = 0;
    characterInit->yCoordinate = 0;

    // Loop through map to place 'uninitialised' cells
    for (int y = 0; y < MAPSIZE; ++y) {
        for (int x = 0; x < MAPSIZE; ++x) {
            mapToInit[x][y] = uninitialised;
        }
    }
    // Place shop in cell (0,0)
    mapToInit[0][0] = shopCell;

    // Place impassable cells at random map locations
    while(impassables != 0){
        srand(time(NULL));
        x = rand() % MAPSIZE;
        y = rand() % MAPSIZE;
        // Check if cell is already populated
        if(mapToInit[x][y].icon != 'U'){
            continue;
        }else{
            mapToInit[x][y] = impassableCell;
            impassables--;
        }
    }

    // Loop through each cell and place a random cell
    for (y = 0; y < MAPSIZE; ++y) {
        for (int x = 0; x < MAPSIZE; ++x) {
            if(mapToInit[x][y].icon != 'U'){
                continue;
            }else{
                // Use probability of each gem/ore to decide what gets placed in the cell
                // Diamond
                gemProbability = (rand() % 100) + 1;
                if(gemProbability <= (gemRarityFactor * diamond.item.rarity * 100)){
                    mapToInit[x][y] = diamond;
                    continue;
                }
                // Gold Ore
                gemProbability = (rand() % 100) + 1;
                if(gemProbability <= (gemRarityFactor * goldOre.item.rarity * 100)){
                    mapToInit[x][y] = goldOre;
                    continue;
                }
                // Iron Ore
                gemProbability = (rand() % 100) + 1;
                if(gemProbability <= (gemRarityFactor * ironOre.item.rarity * 100)){
                    mapToInit[x][y] = ironOre;
                    continue;
                }

                mapToInit[x][y] = ground;
            }
        }
    }
}

// Function to output a formatted map
void displayMap(cell (*mapToDisplay)[MAPSIZE], int characterX, int characterY){
    // Initialise blank output map
    char outputMap[MAPSIZE][MAPSIZE] = {};

    // Place character in correct location
    outputMap[characterX][characterY] = '0';

    // Fill output map with icons
    for (int y = 0; y < MAPSIZE; ++y) {
        for (int x = 0; x < MAPSIZE; ++x) {
            if(outputMap[x][y] != '0') {
                outputMap[x][y] = mapToDisplay[x][y].icon;
            }
        }
    }

    // Output map
    printf(".------------------------------.\n");
    for (int y = 0; y< MAPSIZE; ++y) {
        printf("|");
        for (int x = 0; x < MAPSIZE; ++x) {
            printf("%c  ", outputMap[x][y]);
        }
        printf("|\n");
    }
    printf(".------------------------------.\n");
}

// Function to display player inventory and allow inventory management
void viewInventory(player *characterPlayer){
    int selected=0, inventoryInput, inventorySize = (sizeof(characterPlayer->inventory) / sizeof(mineableItem));
    ViewInventoryStart:
    inventoryInput = 0;

    system("cls");
    // Display player info
    printf("\nPlayer %s stats:\n Energy: %d\n Money: %d\n Weight in bag: %d/%d", characterPlayer->name, characterPlayer->energy, characterPlayer->money, characterPlayer->weightInBag, characterPlayer->maximumWeight);
    printf("\n\nInventory:\n");
    // Loop through inventory and output name
    for (int i = 0; i < inventorySize; ++i) {
        printf("%-9s|", characterPlayer->inventory[i].name);
    }
    printf("\n");
    // Loop to show which item is currently selected
    for (int i = 0; i < inventorySize; ++i) {
        if (i== selected){
            printf("^^^^^^^^^ ");
        } else{
            printf("          ");
        }
    }

    // Show selected item details
    printf("\nSelected item:\n Weight: %d\n Value: %d", characterPlayer->inventory[selected].weight, characterPlayer->inventory[selected].value);

    printf("\n\nUse arrow keys to navigate inventory, D to delete an item from your inventory or E to exit the inventory");
    while(inventoryInput == 0){
        inventoryInput = getKey();
        delay(0.5);
        switch (inventoryInput) {
            case 101:
                // Exit inventory
                break;
            case 100:
                // Check if item at selection
                if (characterPlayer->inventory[selected].value != 0){
                    // Confirm removal of item
                    printf("\nAre you sure you want to delete %s? Yes(1) or No(2)");
                    inventoryInput = 0;
                    while (inventoryInput == 0){
                        inventoryInput = getKey();
                        switch (inventoryInput) {
                            case 49:
                                // Delete item
                                characterPlayer->inventory[selected] = none;
                                printf("\nItem Removed");
                                delay(1);
                                break;
                            case 50:
                                // Cancel
                                break;
                            default:
                                inventoryInput = 0;
                                break;
                        }
                    }
                }
                goto ViewInventoryStart;
            case 77:
                // Move selection right if not at end of list
                if (selected < inventorySize-1){
                    selected++;
                } else{
                    printf("\a");
                }
                goto ViewInventoryStart;
                break;
            case 75:
                // Move selection left if not at beginning of list
                if (selected > 0){
                    selected--;
                } else{
                    printf("\a");
                }
                goto ViewInventoryStart;
                break;
            default:
                inventoryInput = 0;
                break;
        }
    }
}

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