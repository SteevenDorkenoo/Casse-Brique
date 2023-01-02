//#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef BOMBERMANDEM_BASIC_GAME_H
#define BOMBERMANDEM_BASIC_GAME_H

// basic code constant 
#define true 1
#define false 0


#define STATUS_SOLO     0
#define STATUS_SERVER   1
#define STATUS_CLIENT   2
#define STATUS_LOCAL   2
#define STATUS_UNDEFINED 1000

// Map constants
#define WALL_DESTROYABLE 'm'
#define WALL_UNDESTROYABLE 'x'
#define SPAWN_POINT  'p'
#define EMPTY_SPACE  ' '
#define HOLE 'h'
#define BOMB_FIRE '*'
#define BOMB 'b'
#define COLOROUTPUT(string) "\x1b[%dm" string "\x1b[0m"

// Object constants
#define BOMB_DOWN '0'
#define BOMB_UP '1'
#define FLAME_YELLOW '2'
#define FLAME_BLUE '3'
#define FLAME_ROUGE '4'
#define BOMB_THROUGH '5'
#define BOMB_KICK '6'
#define INVINCIBILITY '7'
#define HEART '8'
#define HEALTH '9'

// Movement Constants
#define UP  '8'
#define DOWN  '2'
#define LEFT  '4'
#define RIGHT  '6'

// Action constants
#define MOVE short 0
#define PLACE_BOMB 'p'
#define SKIP  's'

// Map Status constants
#define MAP_NOT_SELECTED 0
#define MAP_SELECTED 1
#define MAP_ALREADY_PLAYED 2

typedef struct {
    unsigned short x;
    unsigned short y;
} Position;

typedef struct {
    Position* position;
    unsigned short remainingTurns;
    unsigned short range;
} Bomb;

typedef struct {
    unsigned short id;
    char symbole;
    char* name;
    int socket;
    unsigned short alive;
    unsigned short bombs;
    short rangeAdd;
    int maxRange;
    unsigned short passeBomb;
    unsigned short bombKick;
    unsigned short invincibility; // number of rounds remaining
    unsigned short heart; //
    unsigned short health;
    int plantedAbomb;  // to check if the player placed a bomb in ten previous turn to return it to hist inventory
    Position* currentPosition;
} Player;

typedef struct {
    unsigned short numberOfPlayers;
    unsigned short bombPerPlayers;
    unsigned short columns;
    unsigned short rows;
    char fileName[30];
    char** layout;
} Map;

typedef struct {
    unsigned short numberOfMaps;
    Map** maps;
} MapList;

typedef struct {
    unsigned short numberOfPlayers;
    Player** playerList;
    unsigned short status;
    struct sockaddr_in address;
    unsigned short port;
    MapList* mapList;
    short* selectedMaps;
} GLobalGame;


typedef struct {
    Bomb** activeBombs;
    unsigned short numberOfActiveBombs;
    Map* currentMap;
    char** state;
    int currentTurn;
    GLobalGame* game;
} CurrentGame;


MapList* loadMaps();

void freeMaplist(MapList* mapList);

Map* loadMap(char* mapFileName);

void freeMap(Map* map);

#endif //BOMBERMANDEM_BASIC_GAME_H
