#include "main_header.h"

//
// Created by lyesj on 21/10/2022.
//

#ifndef BOMBERMANDEM_GAME_INIT_H
#define BOMBERMANDEM_GAME_INIT_H

GLobalGame* initGlobalGame(int argc, char* argv[]);

void freeGlobalGame(GLobalGame* globalGame);

void initPlayers(GLobalGame* globalGame);

void freePlayerList(Player** playerList, unsigned short numberOfPlayers);

Player* initPlayer(char* name, unsigned short id);

void freePlayer(Player* player);

void initPlayersLocal(GLobalGame* globalGame);

void parseArguments(int argc, char* argv[], GLobalGame* globalGame);

short* selectGameMaps(MapList* mapList, unsigned short numberOfPlayers);

void startSession(GLobalGame* GLobalGame);

unsigned short selectNextMap(short* mapRotation, unsigned short numberOfMaps);

char** copyMapLayout(Map* map);

void freeState(char** state, unsigned short columns);

#endif //BOMBERMANDEM_GAME_INIT_H
