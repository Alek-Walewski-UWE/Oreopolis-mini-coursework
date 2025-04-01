//
// Created by Alek.Walewski on 26/03/2025.
//
#include "main.h"

int main(){
    char readChar;
    int mainMenuOption = 0, gameLoop = 1;
    cell map[mapSize][mapSize] = {};
    player character;

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
    while(mainMenuOption == 0){
        mainMenuOption = getKey();
        switch(mainMenuOption){
            case 49:
                // Call initial game setup function
                gameSetup(&map, &character);
                break;
            case 50:
                // Call function to read leaderboard

                goto mainMenu;
                break;
            case 51:
                return 0;
            default:
                mainMenuOption = 0;
                break;
        }
    }

    printf("\nGame initiated successfully, good luck!");
    delay(1);

    // Main game loop
    while(gameLoop==1){
        // Check if character is out of energy and run end game function if true
        if(character.energy == 0){
            //endGame();
            break;
        }

        // Output map function
        system("cls");
        printf("Map:\n");
        displayMap(&map, character.xCoordinate, character.yCoordinate);
        getchar();

    }

    return 0;
}

// Initial game setup function to initialise game variables and structures
void gameSetup(cell (*mapToInit)[mapSize], player *characterInit){
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
    characterInit->money = 0;
    characterInit->xCoordinate = 0;
    characterInit->yCoordinate = 0;

    // Loop through map to place 'uninitialised' cells
    for (int x = 0; x < mapSize; ++x) {
        for (int y = 0; y < mapSize; ++y) {
            mapToInit[x][y] = uninitialised;
        }
    }
    // Place shop in cell (0,0)
    mapToInit[0][0] = shopCell;

    // Place impassable cells at random map locations
    while(impassables != 0){
        srand(time(NULL));
        x = rand() % mapSize;
        y = rand() % mapSize;
        // Check if cell is already populated
        if(mapToInit[x][y].icon != 'U'){
            continue;
        }else{
            mapToInit[x][y] = impassableCell;
            impassables--;
        }
    }

    // Loop through each cell and place a random cell
    for (x = 0; x < mapSize; ++x) {
        for (int y = 0; y < mapSize; ++y) {
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
void displayMap(cell (*mapToDisplay)[mapSize], int characterX, int characterY){
    // Initialise blank output map
    char outputMap[mapSize][mapSize] = {};

    // Place character in correct location
    outputMap[characterX][characterY] = '0';

    // Fill output map with icons
    for (int x = 0; x < mapSize; ++x) {
        for (int y = 0; y < mapSize; ++y) {
            if(outputMap[x][y] != '0') {
                outputMap[x][y] = mapToDisplay[x][y].icon;
            }
        }
    }

    // Output map
    printf(".------------------------------.\n");
    for (int x = 0; x< mapSize; ++x) {
        printf("|");
        for (int y = 0; y < mapSize; ++y) {
            printf("%c  ", outputMap[x][y]);
        }
        printf("|\n");
    }
    printf(".------------------------------.\n");
}