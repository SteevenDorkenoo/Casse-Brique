#include <stdio.h>
#include "includes/game_run.h"
#include "includes/game_init.h"
#include "helper_funcs.h"


int main(int argc, char* argv[]) {
    clear_screen();
    printf("\nLancement du jeu Bombermandem...\n");
    GLobalGame* gLobalGame = initGlobalGame(argc, argv);
    displayPlayers(gLobalGame->playerList, gLobalGame->numberOfPlayers);
    startSession(gLobalGame);
    freeGlobalGame(gLobalGame);
    printf("\nFermeture du jeu Bombermandem...\n");
    return EXIT_SUCCESS;
}
