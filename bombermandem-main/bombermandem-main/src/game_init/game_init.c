#include "../includes/game_init.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "../includes/helper_funcs.h"
#include "game_run.h"

GLobalGame* initGlobalGame(int argc, char* argv[]) {
    GLobalGame* globalGame;
    globalGame = malloc(sizeof(GLobalGame));
    parseArguments(argc, argv, globalGame);
    initPlayers(globalGame);
    globalGame->mapList = loadMaps();
    printf("Après loadmaps\n");
    globalGame->selectedMaps = selectGameMaps(globalGame->mapList, globalGame->numberOfPlayers);
    // TODO input all globalGameAttributes

    return globalGame;
}

void freeGlobalGame(GLobalGame* globalGame) {
    freeMaplist(globalGame->mapList);
    freePlayerList(globalGame->playerList, globalGame->numberOfPlayers);
    if (globalGame->selectedMaps != NULL) {
        free(globalGame->selectedMaps);
    }
    free(globalGame);
}

void initPlayers(GLobalGame* globalGame) {
    switch (globalGame->status) {
        case STATUS_LOCAL:
        case STATUS_SOLO:
            initPlayersLocal(globalGame);
            break;
        default:
            printf("Pas de statut ????");
            break;
    }
}

void freePlayerList(Player** playerList, unsigned short numberOfPlayers) {
    for (int i = 0; i < numberOfPlayers; ++i) {
        freePlayer(playerList[i]);
    }
}

Player* initPlayer(char* name, unsigned short id) {
    Player* newPlayer = malloc(sizeof(Player));
    newPlayer->name = malloc(sizeof(name) + 1);
    strcpy(newPlayer->name, name);
    newPlayer->id = id;
    newPlayer->symbole = id + '0';
    newPlayer->alive = 1;
    newPlayer->bombs = 0;
    newPlayer->rangeAdd = 1;
    newPlayer->passeBomb = 0;
    newPlayer->bombKick = 0;
    newPlayer->invincibility = 0;
    newPlayer->heart = 0;
    newPlayer->health = 5;

    return newPlayer;
}

void freePlayer(Player* player) {
    free(player->name);
    free(player);
}

void initPlayersLocal(GLobalGame* globalGame) {
    int quit;
    char choice[21];

    quit = 0;
    globalGame->playerList = NULL;
    globalGame->numberOfPlayers = 0;
    while (1) {
        clear_screen();
        printf("\n\tJoueurs inscrits actuellement: ");
        for (int i = 0; i < globalGame->numberOfPlayers; ++i) {
            printf("\n\t\tJoeeur numéro %d, nom:%s\n", i + 1, globalGame->playerList[i]->name);
        }
        printf("\n\nVoulez-vous ajouter un joueur ?\n\t- Y pour oui\n\t- N pour non\n:");
        while (1) {
            int addPlayer = 0;
            scanf("%c", choice);
            clearInput();
            if (choice[0] != 'Y' && choice[0] != 'y' && choice[0] != 'N' && choice[0] != 'n') {
                printf("\nLes seuls choix possibles sont Y et N pour oui et non.\n");
            }
            switch (choice[0]) {
                case 'y':
                case 'Y':
                    addPlayer = 1;
                    break;
                case 'N':
                case 'n':
                    quit = 1;
                    break;
                default:
                    printf("\nLes seuls choix possibles sont Y et N pour oui et non.\n");
                    break;
            }
            if (addPlayer) {
                break;
            }
            if (quit) {
                break;
            }
        }
        if (quit) {
            break;
        }
        printf("\n\nVeuillez entrer votre nom (taille de 20 caractères maximum):\n->");
        inputString(choice, 20);
        globalGame->numberOfPlayers++;
        printf("\nBienvenue %s!\nNombre actuel de joueur:%d", choice, globalGame->numberOfPlayers);
        globalGame->playerList = realloc(globalGame->playerList, (sizeof(Player*) * globalGame->numberOfPlayers) + 1);
        globalGame->playerList[globalGame->numberOfPlayers - 1] = initPlayer(choice, globalGame->numberOfPlayers - 1);
    }
    printf("\nFIn du while\n");
}

MapList* loadMaps() {
    // TODO finir de debug ici bruh
    struct dirent* dirContent;
    DIR* directory;
    MapList* mapList;

    mapList = malloc(sizeof(MapList));
    mapList->numberOfMaps = 0;
    mapList->maps = NULL;
    directory = opendir("./maps/");
    if (directory == NULL) {
        printf("\nErreur dans la lecture des cartes.\n");
        return 0;
    }
    while ((dirContent = readdir(directory)) != NULL) {
        if (dirContent->d_type != DT_REG) continue;
        mapList->numberOfMaps++;
        mapList->maps = realloc(mapList->maps, (sizeof(Map) * mapList->numberOfMaps) + 1);
        mapList->maps[mapList->numberOfMaps - 1] = loadMap(dirContent->d_name);
    }
    closedir(directory);
    for (int i = 0; i < mapList->numberOfMaps; ++i) {
    }

    return mapList;
}

void freeMaplist(MapList* mapList) {
    if (mapList == NULL) {
        return;
    }
    for (int i = 0; i < mapList->numberOfMaps; ++i) {
        // freeMap(mapList->maps[i]);
    }
    free(mapList);
}

Map* loadMap(char* mapFileName) {
    Map* map;
    FILE* mapFile;
    char buffer[100];
    unsigned int lineNumber;
    unsigned int layoutLineNumber;
    unsigned int spawnPoints;

    map = malloc(sizeof(Map));
    strcpy(map->fileName, "./maps/");
    strcat(map->fileName, mapFileName);
    // TODO load a map (with all attributes)
    printf("\nfichier: %s\n", map->fileName);
    if ((mapFile = fopen(map->fileName, "r")) == NULL) {
        printf("Erreur lors de la lecture de la carte");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    lineNumber = 0;
    layoutLineNumber = 0;
    spawnPoints = 0;
    while (fgets(buffer, 100, mapFile) != NULL) {
        ++lineNumber;
        if (lineNumber == 1) {
            map->bombPerPlayers = strtol(buffer, NULL, 10);
            continue;
        }
        if (lineNumber == 2) {
            char* end = NULL;
            map->columns = strtol(buffer, &end, 10);
            map->rows = strtol(end, &end, 10);
            map->layout = malloc(sizeof(char*) * (map->rows + 1));
            for (int i = 0; i < map->rows; ++i) {
                map->layout[i] = malloc(sizeof(char) * (map->columns + 1));
            }
            continue;
        }
        // Copie du layout de la map
        for (int i = 0; i < map->columns; ++i) {
            if (buffer[i] == 'p') {
                spawnPoints++;
            }
            strcpy(map->layout[layoutLineNumber] + i, &buffer[i]);
        }
        layoutLineNumber++;
    }
    map->numberOfPlayers = spawnPoints;
    fclose(mapFile);
    printf("La map en mémoire\n");
    printf("Bombe par joueurs:%d\n", map->bombPerPlayers);
    printf("Columns:%d, rows:%d\n", map->columns, map->rows);
    printf("Max players:%d\n\n", map->numberOfPlayers);
    for (int i = 0; i < map->rows; ++i) {
        printf("%s", map->layout[i]);
    }

    return map;
}

void freeMap(Map* map) {
    free(map);
}

short* selectGameMaps(MapList* mapList, unsigned short numberOfPlayers) {
    short* selectedMaps;
    char choice[20];
    int intChoice;

    selectedMaps = malloc(sizeof(short) * mapList->numberOfMaps + 1);
    for (size_t i = 0; i < mapList->numberOfMaps; i++) {
        selectedMaps[i] = MAP_NOT_SELECTED;
    }

    // TODO exclude unselected maps from the maplist (don't forget to free)
    while (1) {
        system("clear");
        printf("\nSélectionnez les cartes que vous voulez avoir dans votre rotation\n\to: non sélectionnée, * sélectionnée\n\tNombre de joueurs max:%d ",
               numberOfPlayers);
        for (unsigned short i = 0; i < mapList->numberOfMaps; i++) {
            printf("\n%s carte n°%d, nb de joueurs max:%d", (selectedMaps[i] ? "\U00002714" : "\U0000274C"), i + 1,
                   mapList->maps[i]->numberOfPlayers);
        }
        printf("\n\tinscrire le numéro de la carte à sélectionner, ex: 1 ou 2. Q pour terminer la sélection:\n");
        scanf("%c", choice);
        clearInput();
        if (choice[0] == 'q') {
            break;
        }
        intChoice = atoi(choice);
        if (intChoice <= 0 || intChoice > mapList->numberOfMaps) {
            printf("\nVeuillez entrer un numéro de carte valide.\n");
            fflush(stdout);
            sleep(3);
        } else if (mapList->maps[intChoice - 1]->numberOfPlayers < numberOfPlayers) {
            printf("\nTrop de joueurs pour jouer sur cette carte!.\n");
            fflush(stdout);
            sleep(3);

        } else {
            selectedMaps[intChoice - 1] = !selectedMaps[intChoice - 1];
        }
    }
    clear_screen();
    printf("\nFin de la sélection des cartes de jeu.\n");

    return selectedMaps;
}

void parseArguments(int argc, char** argv, GLobalGame* globalGame) {
    int opt;
    globalGame->status = STATUS_UNDEFINED;
    while ((opt = getopt(argc, argv, "sSPLp:h:")) != -1) {
        switch (opt) {
            case 's':
                globalGame->status = STATUS_SOLO;
                printf("Partie en solo sélectionnée\n");
                break;
            case 'h':
                globalGame->address.sin_addr.s_addr = inet_addr(optarg);
                printf("Adresse de l'hôte renseignée est %s\n", inet_ntoa(globalGame->address.sin_addr));
                break;
            case 'p':
                globalGame->address.sin_port = atoi(optarg);
                printf("Port de l'hôte renseigné:%d\n", globalGame->address.sin_port);
                break;
            case 'P':
                globalGame->status = STATUS_CLIENT;
                printf("Partie multijoueur en tant que joueur sélectionnée\n");
                break;
            case 'S':
                globalGame->status = STATUS_CLIENT;
                printf("Partie multijoueur en tant que Serveur sélectionnée\n");
                break;
            case 'L':
                globalGame->status = STATUS_LOCAL;
                printf("Partie multijoueur en local sélectionnée\n");
            case '?':
                break;
            default:
                printf("wtf");
                break;
        }
    }
    if (globalGame->status == STATUS_UNDEFINED) {
        globalGame->status = STATUS_SOLO;
        printf("Partie en solo sélectionnée\n");
    }
    sleep(3);
}

void startSession(GLobalGame* globalGame) {
    unsigned short nextMap;
    unsigned short activeSession;

    activeSession = 0;
    do {
        nextMap = selectNextMap(globalGame->selectedMaps, globalGame->mapList->numberOfMaps);
        printf("Lancement de la partie sur la carte n°%d\n", nextMap);
        CurrentGame* currentGame = malloc(sizeof(CurrentGame));
        currentGame->game = globalGame;
        currentGame->numberOfActiveBombs = 0;
        currentGame->currentTurn = 0;
        currentGame->currentMap = globalGame->mapList->maps[nextMap];
        currentGame->state = copyMapLayout(currentGame->currentMap);
        launchGame(currentGame);
        freeState(currentGame->state, currentGame->currentMap->columns);
    } while (activeSession < globalGame->mapList->numberOfMaps);
}

unsigned short selectNextMap(short* mapRotation, unsigned short numberOfMaps) {
    unsigned short choice;


    do {
        choice = rand() % numberOfMaps;
    } while (mapRotation[choice] != MAP_SELECTED);
    mapRotation[choice] = MAP_ALREADY_PLAYED;

    return choice;
}

char** copyMapLayout(Map* map) {
    char** newLayout;

    newLayout = malloc(sizeof(char*) * map->rows);
    for (int i = 0; i < map->rows; ++i) {
        newLayout[i] = malloc(sizeof(char) * map->columns);
        for (int j = 0; j < map->columns; ++j) {
            newLayout[i][j] = map->layout[i][j];
        }
    }

    return newLayout;
}

void freeState(char** state, unsigned short columns) {
    for (int i = 0; i < columns; ++i) {
        free(state[i]);
    }
    free(state);
}
