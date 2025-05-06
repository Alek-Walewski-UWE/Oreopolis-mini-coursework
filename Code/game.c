//
// Created by Alek.Walewski on 06/05/2025.
//

#include "game.h"
#include "player.h"

FILE* currentFile;
time_t startTime;

// Main function that runs throughout the game
int gameLoop(){
    char readChar;
    int userInput = 0;
    cell map[MAPSIZE][MAPSIZE] = {};
    mineableItem currentItem;
    player character = {};
    srand(time(NULL));

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
                viewLeaderboard();
                userInput = 0;
                goto mainMenu;
            case 51:
                return 0;
            default:
                userInput = 0;
                break;
        }
    }

    time(&startTime);
    // Main game loop
    while(1){
        // Check if character is out of energy and run end game function if true
        if(character.energy <= 0){
            endGame(&character, 2);
            break;
        }

        // Output map function
        system("cls");
        displayMap(&map, character.xCoordinate, character.yCoordinate);

        // Output player stats
        showPlayerStats(&character);

        // Output player options
        printf("\nUse arrow keys to move in selected direction, I for inventory");
        // Display shop or mining option only if standing on a suitable cell
        if(map[character.xCoordinate][character.yCoordinate].icon == '$'){
            printf(" or E to open the shop");
            character.energy = character.defaultEnergy;
        } else if(map[character.xCoordinate][character.yCoordinate].mineable == 1){
            currentItem = map[character.xCoordinate][character.yCoordinate].item;
            printf(" or E to mine item: \n %s\n Value: %d\n Weight: %d", currentItem.name, currentItem.value, currentItem.weight);
        }


        // Wait valid user input
        userInput = 0;
        while(userInput == 0){
            userInput = getKey();
            switch (userInput) {
                case 105:
                    // View Inventory
                    viewInventory(&character);
                    break;
                case 101:
                    // Mine item
                    if (map[character.xCoordinate][character.yCoordinate].mineable){
                        mineItem(map, &character);
                    }else if(map[character.xCoordinate][character.yCoordinate].icon == '$'){
                        // View shop
                        openShop(&character);
                        if(character.money >= MONEYTOWIN){
                            endGame(&character, 1);
                            return 0;
                        }
                    }
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
                strcpy(&characterInit->selectedDifficulty,"Easy");
                gemRarityFactor = easyRarityFactor;
                impassables = easyImpassables;
                characterInit->energy = easyPlayerEnergy;
                characterInit->defaultEnergy = easyPlayerEnergy;
                break;
            case 50:
                // Initiate normal difficulty variables
                strcpy(&characterInit->selectedDifficulty,"Medium");
                gemRarityFactor = normalRarityFactor;
                impassables = normalImpassables;
                characterInit->energy = normalPlayerEnergy;
                characterInit->defaultEnergy = normalPlayerEnergy;
                break;
            case 51:
                // Initiate hard mode difficulty
                strcpy(&characterInit->selectedDifficulty,"Hard");
                gemRarityFactor = hardRarityFactor;
                impassables = hardImpassables;
                characterInit->energy = hardPlayerEnergy;
                characterInit->defaultEnergy = hardPlayerEnergy;
                break;
            default:
                difficulty = 0;
                break;
        }
    }

    // Set character variables that are the same for every difficulty
    characterInit->maximumWeight = 10;
    characterInit->weightInBag = 0;
    characterInit->money = 0;
    characterInit->xCoordinate = 0;
    characterInit->yCoordinate = 0;
    characterInit->itemsInInventory = 0;

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
    printf(".------------------------------------.\n");
    for (int y = 0; y< MAPSIZE; ++y) {
        printf("|");
        for (int x = 0; x < MAPSIZE; ++x) {
            printf("%c  ", outputMap[x][y]);
        }
        printf("|\n");
    }
    printf(".------------------------------------.\n");
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
                    printf("\nAre you sure you want to delete %s? Yes(1) or No(2)", characterPlayer->inventory[selected].name);
                    inventoryInput = 0;
                    while (inventoryInput == 0){
                        inventoryInput = getKey();
                        switch (inventoryInput) {
                            case 49:
                                // Move last item to fill deleted items place
                                if(selected != characterPlayer->itemsInInventory){
                                    characterPlayer->inventory[selected] = characterPlayer->inventory[characterPlayer->itemsInInventory - 1];
                                    characterPlayer->inventory[characterPlayer->itemsInInventory - 1] = none;
                                }else{
                                    characterPlayer->inventory[selected] = none;
                                }
                                characterPlayer->itemsInInventory--;
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

// Function to allow selling and upgrading in shop
void openShop(player *shopPlayer){
    int shopSelection, moneyToReceive;

    ShopView:
    shopSelection = 0, moneyToReceive = 0;
    system("cls");
    showPlayerStats(shopPlayer);
    printf("\nPress (1) to view the sell section of the shop, (2) to view the upgrade section or (E) to exit the shop.");
    while (shopSelection == 0){
        shopSelection = getKey();
        switch (shopSelection) {
            case 49:
                // Sell section of shop
                system("cls");
                showPlayerStats(shopPlayer);

                // Calculate money to receive
                for (int i = 0; i < (sizeof(shopPlayer->inventory) / sizeof(mineableItem)); ++i) {
                    moneyToReceive += shopPlayer->inventory[i].value;
                }

                printf("\nWould you like to sell all your items? You will receive %d money.\n(1) Yes or (2) No", moneyToReceive);
                shopSelection = 0;

                while(shopSelection == 0){
                    shopSelection = getKey();
                    switch (shopSelection) {
                        case 49:
                            // Sell items
                            shopPlayer->money += moneyToReceive;
                            // Reset items in inventory
                            for (int i = 0; i < (sizeof(shopPlayer->inventory) / sizeof(mineableItem)); ++i) {
                                shopPlayer->inventory[i] = none;
                            }
                            shopPlayer->itemsInInventory = 0;
                            shopPlayer->weightInBag = 0;
                            printf("\nItems sold successfully!");
                            delay(1.5);

                        case 50:
                            // Cancel sell
                            break;
                        default:
                            shopSelection = 0;
                            break;
                    }
                }
                goto ShopView;
            case 50:
                // Upgrade section of shop
                system("cls");
                showPlayerStats(shopPlayer);
                if (bag1.available){
                    printf("\nUpgrades available: \n (1) %s: \n  Current weight capacity: %d \n  New weight capacity: %d \n  Upgrade Cost: %d", bag1.name, shopPlayer->maximumWeight, bag1.value, bag1.cost);

                    shopSelection = 0;
                    while (shopSelection == 0){
                        shopSelection = getKey();
                        switch (shopSelection){
                            case 49:
                                // Upgrade bag
                                if (shopPlayer->money >= bag1.cost){
                                    shopPlayer->maximumWeight = bag1.value;
                                    shopPlayer->money -= bag1.cost;
                                    bag1.available = 0;
                                    printf("\nBag upgraded successfully!");
                                    delay(1.5);
                                }else{
                                    printf("\nNot enough money to upgrade");
                                    delay(1.5);
                                }
                                break;
                            default:
                                shopSelection = 0;
                                break;
                        }
                    }
                } else{
                    printf("\nNo upgrades available!");
                    delay(1.5);
                }

                goto ShopView;
            case 101:
                // Exit shop
                return;
            default:
                shopSelection = 0;
                break;
        }
    }
}

// Function to end the game
void endGame(player *endPlayer, int endState){
    float timeTaken, secsTaken;
    int minsTaken;
    system("cls");
    timeTaken = difftime(time(NULL), startTime);
    minsTaken = timeTaken / 60;
    secsTaken = timeTaken - (minsTaken*60);

    switch (endState) {
        case 1:
            // Win
            printf("\nCongratulations you have won the game!");
            currentFile = fopen(leaderboardFile, "a+");

            fseek(currentFile,0,SEEK_END);
            if (ftell(currentFile) == 0){
                fprintf(currentFile,"%s",LEADERBOARDHEADER);
            }

            fprintf(currentFile, "%-9s|%-10s|%d:%.0f\n", endPlayer->name, endPlayer->selectedDifficulty, minsTaken, secsTaken);

            fclose(currentFile);
            break;
        case 2:
            // Lose
            printf("\nYou have lost the game!");
            break;
        default:
            break;
    }
    showPlayerStats(endPlayer);
    printf("\nTime taken: %d mins %.0f seconds", minsTaken, secsTaken);
    printf("\nPress enter to end the game");
    getch();
}

void viewLeaderboard(){
    char readChar;
    system("cls");
    // Open file with leaderboard
    currentFile = fopen(leaderboardFile, "r");
    // Loop through and print each char until end of file is reached
    while((readChar = (char)fgetc(currentFile)) != EOF){
        printf("%c", readChar);
    }
    fclose(currentFile);
    printf("\nPress enter to exit the leaderboard");
    getch();
}