//
// Created by Alek.Walewski on 26/03/2025.
//
#include "main.h"

int main(){
    char readChar;
    int mainMenuOption = 0;
    cell map[mapSize][mapSize] = {};

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
                gameSetup(&map);
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
    return 0;
}

void gameSetup(cell (*mapToInit)[mapSize]){
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
                break;
            case 50:
                // Initiate normal difficulty variables
                gemRarityFactor = normalRarityFactor;
                impassables = normalImpassables;
                break;
            case 51:
                // Initiate hard mode difficulty
                gemRarityFactor = hardRarityFactor;
                impassables = hardImpassables;
                break;
            default:
                difficulty = 0;
                break;
        }
    }

    // Place shop in cell (0,0)

    for (int x = 0; x < mapSize; ++x) {
        for (int y = 0; y < mapSize; ++y) {
            mapToInit[x][y] = uninitialised;
        }
    }
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