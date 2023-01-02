#include "main_header.h"

#ifndef BOMBERMANDEM_GAME_RUN_H
#define BOMBERMANDEM_GAME_RUN_H

void display(Map *map) ;

void playersPositionInit(CurrentGame *currentGame) ;

void launchGame(CurrentGame* currentGame);

void allPlayerActions(CurrentGame *currentGame);

void movePlayer(Player* player, Map* map , char movement);

int checkWinner(CurrentGame* currentGame);

void printWinner(CurrentGame* currentGame);

int canMove(Player *Player , Map* map , int movement) ; 

int cantMoveReason(Map * map , int selectedRow , int selectedCol) ;

void plantBom(Map *map , Player *player , Bomb **activeBombs , CurrentGame *currentGame)  ;

void checkBombs(Bomb** activeBombs, int numberOfActiveBombs , Map *map , Player **playersList , int numberOfPlayers) ;

void returnPlayersBombsToInventorys(Player **playersList , int numbreOfPlayers) ;  

void clearBombFires (Map *map) ; 

void exploseActiveBomb(Bomb *bomb , Map* map , Player **playersList , int numberOfPlayers , Bomb **activeBombs , int numberOfActiveBombs)  ;

void checkIfPlayerInBombRange(Player **playersList , int numberOfPlayers , int x, int y , Bomb *bomb)  ;

void checkObjectInDestroyablWall(Player *player , Map *map , int x , int y) ;

void checkInvincibilityByTurn (Player **playersList , int numberOfPlayers) ;

void test(int *nbr) ;

#endif //BOMBERMANDEM_GAME_RUN_H
