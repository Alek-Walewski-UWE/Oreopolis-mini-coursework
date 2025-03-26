//
// Created by Alek.Walewski on 26/03/2025.
//
#include "main.h"

int main(){
    char mainMenuOption;
    char readChar;

    mainMenu:
    mainMenuOption = 0;
    readChar = 0;
    system("cls");
    // Open file with introduction text
    currentFile = fopen(introductionFile, "r");
    // Loop through and print each char until end of file is reached
    while((readChar = fgetc(currentFile)) != EOF){
        printf("%c", readChar);
    }
    fclose(currentFile);

    // Allow user to choose menu option
    while(mainMenuOption == NULL){
        if(kbhit){
            mainMenuOption = getch();
            switch((int)mainMenuOption){
                case 49:
                    // Prompt user to select difficulty
                    // Call initial game setup function
                    break;
                case 50:
                    // Call function to read leaderboard
                    break;
                case 51:
                    return 0;
                default:
                    mainMenuOption = NULL;
                    break;
            }
        }
    }


    return 0;
}