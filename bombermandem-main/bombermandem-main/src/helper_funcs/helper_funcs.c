#include <stdio.h>
#include <string.h>
#include "helper_funcs.h"


int inputString(char* stringToEnter, int length) {
    fflush(stdin);
    char* start = NULL;

    if (fgets(stringToEnter, length, stdin) != NULL) {
        start = strchr(stringToEnter, '\n');
        if (start != NULL) {
            *start = '\0';
        }
        return 1;
    } else {
        return 0;
    }
    clearInput();
}

void displayPlayers(Player** playerList, int numberOfPlayers) {
    for (int i = 0; i < numberOfPlayers; ++i) {
        printf("Jouer numéro %d, \n\tnom:%s\n", i + 1, playerList[i]->name);
    }
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void clear_screen() {
#ifdef WINDOWS
    system("cls");
#else
    // Assume POSIX
    system("clear");
#endif
}
