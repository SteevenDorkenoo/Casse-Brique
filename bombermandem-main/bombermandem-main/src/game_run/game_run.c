#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/game_run.h"
#include <time.h>
#include "../includes/helper_funcs.h"


void display(Map* map) {
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if (map->layout[i][j] == 'x') {
                printf("\U00002B1B");
            } else if (map->layout[i][j] == 'm') {
                printf("\U00002B1C");
            } else if (map->layout[i][j] == 'p') {
                printf("\U0001F620");
            } else if (map->layout[i][j] == ' ') {
                printf(" ");
            } else if (map->layout[i][j] == 'h') {
                printf("\U0001F365");
            } else if (map->layout[i][j] == 'b') {
                printf("\U0001F4A3");
            } else if (map->layout[i][j] == '*') {
                printf("\U0001F386");
            }

                //bonus objects
            else if (map->layout[i][j] == '0') {
                printf("\U00002796");
            } else if (map->layout[i][j] == '1') {
                printf("\U00002795");
            } else if (map->layout[i][j] == '2') {
                printf("\U0001F7E1");
            } else if (map->layout[i][j] == '3') {
                printf("\U0001F535");
            } else if (map->layout[i][j] == '4') {
                printf("\U0001F534");
            } else if (map->layout[i][j] == '5') {
                printf("\U0001F51B");
            } else if (map->layout[i][j] == '6') {
                printf("\U0001F9B6");
            } else if (map->layout[i][j] == '7') {
                printf("\U00002B50");
            } else if (map->layout[i][j] == '8') {
                printf("\U0001F49F");
            } else if (map->layout[i][j] == '9') {
                printf("\U0001F495");
            } else {
                printf("\n error caracters not found");
                break;
            }
        }
        printf("\n");
    }
}


void launchGame(CurrentGame* currentGame) {

    // inisializing players positions on the map
    playersPositionInit(currentGame) ;         
    int winner;
    // main game loop 
    while (1) {

        // allocating space for the bombs array at the begenning of every turn 
        currentGame->activeBombs = malloc(sizeof(Bomb*) * (currentGame->game->numberOfPlayers * 2)) ;  
        currentGame->numberOfActiveBombs = 0 ; 
        // function to handel all players choises and all players turns in the game
        allPlayerActions(currentGame);

        // checking and explosing active bombs in the map 
       checkBombs(currentGame->activeBombs , currentGame->numberOfActiveBombs , currentGame->currentMap , currentGame->game->playerList , currentGame->game->numberOfPlayers) ; 
       returnPlayersBombsToInventorys(currentGame->game->playerList , currentGame->game->numberOfPlayers) ; 

      
        display(currentGame->currentMap);
        
        // we stop for to seconds to show the bombs explosions 
        sleep(2) ; 
        clear_screen() ; 
        // changing bombs explosion cells to holes 
        clearBombFires(currentGame->currentMap);

        checkInvincibilityByTurn(currentGame->game->playerList , currentGame->game->numberOfPlayers) ;

         winner = checkWinner(currentGame) ; 
        if (winner == true) {
             break;
         }
    // freeing the array of avive bombs at the end 
    free (currentGame->activeBombs) ;     
    }
    
}


void allPlayerActions(CurrentGame * currentGame) {
    char playerChoise ;
    int move ;
    clear_screen();
    display(currentGame->currentMap) ;
    // loop of players turns (the loop start at 1 and stops at the last player)
    for (int i = 1 ; i < currentGame->game->numberOfPlayers + 1 ; ++i) {
        // first we check if the cureent player is allive or not if allive we ask for his chois if not the loop continue
        if (currentGame->game->playerList[i - 1]->alive = false) {
            continue;
        } else {
            // defining dynamicly the curent players color 
            int playerColor = 31 + i;
            // asking for player input
            printf(COLOROUTPUT(
                           "Player %d Turn to move 8<Up> 2<down> 4<right> 6<left> <p> to plante bomb <s> to skip turn \nname : %s \nhealth : %d \nbombs : %d \n-> "),
                   playerColor, i ,currentGame->game->playerList[i - 1]->name , currentGame->game->playerList[i - 1]->health , currentGame->game->playerList[i - 1]->bombs);
            if (i == 1) {
            playerChoise = getchar();
            getchar();
            } else {
                srand(time(NULL)) ;
                char moves[] = {UP , RIGHT , LEFT , DOWN ,SKIP ,PLACE_BOMB} ;
                int randomChoise = rand() % 6 ; 
                playerChoise = moves[randomChoise] ;

            }
            //case of chosing to move to a specific cell 
            if (playerChoise == DOWN || playerChoise == UP || playerChoise == RIGHT || playerChoise == LEFT) {
                //
                    if (playerChoise == UP) {
                        printf("\nPlayer %d moved <UP> \n" , i) ;
                    } else if (playerChoise == DOWN) {
                        printf("\nPlayer %d moved <DOWN> \n" , i) ;
                    } else if (playerChoise == RIGHT) {
                        printf("\nPlayer %d moved <RIGHT> \n" , i) ;
                    } else {
                        printf("\nPlayer %d moved <LEFT> \n" , i) ;
                    }
                 // we check if the player can move to the position 
                move = canMove(currentGame->game->playerList[i - 1] , currentGame->currentMap , playerChoise) ;
                printf("%d" , move);
                // moving player
                if (move == true) {
                  movePlayer(currentGame->game->playerList[i - 1], currentGame->currentMap , playerChoise); 
                } else {
                    i = i -1;
                }
                sleep(2) ;

                // case of planting a bomb
            } else if (playerChoise == PLACE_BOMB) {
                printf("planting bomb of %d player \n" , i  ) ;
                plantBom(currentGame->currentMap , currentGame->game->playerList[i - 1] , currentGame->activeBombs , currentGame) ; 
                sleep(2);
                
            } else if (playerChoise == SKIP) {
                printf("Player %d has skipped his turn\n" , i) ; 
                sleep(2) ;
                // case of wrong input 
            } else {
                printf("%s" , "wrong input \n") ;
                i = i -1  ;
            }

        }

    }
}

void playersPositionInit(CurrentGame *currentGame) {

    for (int i = 0 ; i < currentGame->game->numberOfPlayers ; i++) {
        currentGame->game->playerList[i]->currentPosition = malloc(sizeof(Position)) ; 
    }
    
    int k = 0 ; 
    for (int i = 0 ; i < currentGame->currentMap->rows ; i++) 
    {
        for (int j = 0 ; j < currentGame->currentMap->columns ; j++) 
        {
            if (currentGame->currentMap->layout[i][j] == 'p') 
            {
                if (k < currentGame->game->numberOfPlayers) {
                currentGame->currentMap->layout[i][j] = currentGame->game->playerList[k]->symbole ; 
                currentGame->game->playerList[k]->currentPosition->x = i ; 
                currentGame->game->playerList[k]->currentPosition->y = j ;

                currentGame->game->playerList[k]->bombs = currentGame->currentMap->bombPerPlayers;
                k++ ;
                } else {
                    currentGame->currentMap->layout[i][j] = EMPTY_SPACE ;
                }

            }

        }
    }
}

void movePlayer(Player* player, Map* map , char movement) {
    int selectedRow = player->currentPosition->x ; 
    int selectedCol = player->currentPosition->y  ;

    map->layout[selectedRow][selectedCol] = EMPTY_SPACE ; 
    /*
    * moving the player to the selected cell first we check the movement of the player up down left right 
    * then we update the map we put the player id in the cell 
    * finaly we update the player current position to use it in the next game 
    */
    switch (movement)
    {
    case UP : 
        selectedRow -=  player->rangeAdd ; 
        if (selectedRow < map->rows && selectedRow > 0) {
        map->layout[selectedRow][selectedCol] = player->symbole ; 
        player->currentPosition->x = selectedRow ; 
        player->currentPosition->y = selectedCol ; 
        checkObjectInDestroyablWall(player , map , selectedRow , selectedCol) ;
        } else {
            printf("can't move outside the map") ; 
        }
        break;
    case LEFT :
         selectedCol -=  player->rangeAdd ; 
        map->layout[selectedRow][selectedCol] = player->symbole ; 
        player->currentPosition->x = selectedRow ; 
        player->currentPosition->y = selectedCol ; 
        checkObjectInDestroyablWall(player , map , selectedRow , selectedCol) ;
        break;
    case RIGHT :
        selectedCol +=  player->rangeAdd ; 
         map->layout[selectedRow][selectedCol] = player->symbole ; 
         player->currentPosition->x = selectedRow ; 
         player->currentPosition->y = selectedCol ; 
        checkObjectInDestroyablWall(player , map , selectedRow , selectedCol) ;
        break;
    
    case DOWN:
        selectedRow +=  player->rangeAdd ; 
        map->layout[selectedRow][selectedCol] = player->symbole ; 
        player->currentPosition->x = selectedRow ; 
        player->currentPosition->y = selectedCol ; 
        //checkObjectInDestroyablWall(player , map , selectedRow , selectedCol) ;
        break;
    
    default:
        break;
    }
}

void checkBombs(Bomb** activeBombs, int numberOfActiveBombs , Map *map , Player **playersList , int numberOfPlayers) 
{
    
    printf("\nChecking for players active Bombs\n");
    printf("%d" , numberOfActiveBombs);
    /**
     * 1 - looking for bombs in the map  
     * 2 - comparing bombs position in the activeBombs array with the position in the map 
     * 3 - explosing every bomb in the map with it range stocked in the bomb struct in the activeBombs array
    */
   
      for (int i = 0 ; i < map->rows ; i++) 
      {
        for (int j = 0 ; j < map->columns ; j++) {
            if (map->layout[i][j] == BOMB) 
            {
                for (int k = 0 ; k < numberOfActiveBombs ;k++) 
                {
                    if (activeBombs[k]->position->x == i && activeBombs[k]->position->y == j) 
                    exploseActiveBomb(activeBombs[k] , map , playersList , numberOfPlayers , activeBombs , numberOfActiveBombs) ;
                } 
            }
        }
      }
    }


void exploseActiveBomb(Bomb *bomb , Map* map , Player **playersList , int numberOfPlayers , Bomb **activeBombs , int numberOfActiveBombs) 
{
  
for (int i = 0 ; i <= bomb->range ; i++) 
    {
        if (bomb->position->x + i <= map->rows && bomb->position->x + i > 0) 
        {
            // destroyable walls
            if (map->layout[bomb->position->x + i][bomb->position->y] == WALL_DESTROYABLE ) {
                
                srand(time(NULL)) ; 

                int objectCode = (rand() % (51)) ;
                if (objectCode > 10) {
                    map->layout[bomb->position->x +i][bomb->position->y] = BOMB_FIRE ; 
                } else {
                    char display[4] ; 
                    sprintf(display , "%d" , objectCode) ;
                     map->layout[bomb->position->x +i][bomb->position->y] = (char)display[0] ; 
                }
            } 
            // undestroyabl walls 
            if (map->layout[bomb->position->x + i][bomb->position->y] == WALL_UNDESTROYABLE) {
                continue;
            }

            checkIfPlayerInBombRange(playersList ,  numberOfPlayers , bomb->position->x + i , bomb->position->y , bomb) ;
          
        }
    }
    // DOWN 

     for (int i = 0 ; i <= bomb->range ; i++) 
    {
        if (bomb->position->x - i <= map->rows && bomb->position->x - i > 0) 
        {
            // destroyable walls
            if (map->layout[bomb->position->x - i][bomb->position->y] == WALL_DESTROYABLE ) {
                
                srand(time(NULL)) ; 

                int objectCode = (rand() % (51)) ;
                if (objectCode > 10) {
                    map->layout[bomb->position->x - i][bomb->position->y] = EMPTY_SPACE ; 
                } else {
                    char display[4] ; 
                    sprintf(display , "%d" , objectCode) ;
                     map->layout[bomb->position->x - i][bomb->position->y] = (char)display[0] ; 
                }
            } 
            // undestroyabl walls 
            if (map->layout[bomb->position->x - i][bomb->position->y] == WALL_UNDESTROYABLE) {
                continue;
            }

            checkIfPlayerInBombRange(playersList ,  numberOfPlayers , bomb->position->x - i , bomb->position->y , bomb) ;

          
          
        }
    }
    // RIGHT 

     for (int i = 0 ; i <= bomb->range ; i++) 
    {
        if (bomb->position->y + i <= map->columns && bomb->position->y + i > 0) 
        {
            // destroyable walls
            if (map->layout[bomb->position->x][bomb->position->y + i] == WALL_DESTROYABLE ) {
                
                srand(time(NULL)) ; 

                int objectCode = (rand() % (51)) ;
                if (objectCode > 10) {
                    map->layout[bomb->position->x][bomb->position->y + i] = EMPTY_SPACE ; 
                } else {
                    char display[4] ; 
                    sprintf(display , "%d" , objectCode) ;
                     map->layout[bomb->position->x][bomb->position->y + i] = (char)display[0] ; 
                }
            } 
            // undestroyabl walls 
            if (map->layout[bomb->position->x][bomb->position->y + i] == WALL_UNDESTROYABLE) {
                continue;
            }

           
           
           checkIfPlayerInBombRange(playersList ,  numberOfPlayers , bomb->position->x , bomb->position->y + i , bomb) ;

         
          
        }
    }

    for (int i = 0 ; i <= bomb->range ; i++) 
    {
        if (bomb->position->y - i <= map->columns && bomb->position->y - i > 0) 
        {
            // destroyable walls
            if (map->layout[bomb->position->x][bomb->position->y - i] == WALL_DESTROYABLE ) {
                
                srand(time(NULL)) ; 

                int objectCode = (rand() % (51)) ;
                if (objectCode > 10) {
                    map->layout[bomb->position->x][bomb->position->y - i] = EMPTY_SPACE ; 
                } else {
                    char display[4] ; 
                    sprintf(display , "%d" , objectCode) ;
                     map->layout[bomb->position->x][bomb->position->y - i] = (char)display[0] ; 
                }
            } 
            // undestroyabl walls 
            if (map->layout[bomb->position->x][bomb->position->y - i] == WALL_UNDESTROYABLE) {
                continue;
            }

            checkIfPlayerInBombRange(playersList ,  numberOfPlayers , bomb->position->x , bomb->position->y - i , bomb) ;
        }
    }
  
}


void checkObjectInDestroyablWall(Player *player , Map *map , int x , int y) 
{
    if (map->layout[x][y] == BOMB_DOWN) {
        player->bombs-- ;
        printf("inventory capacity - 1\n") ; 
    }

    if (map->layout[x][y] == BOMB_UP) {
        player->bombs++ ; 
        printf("inventory capacity + 1\n") ; 
    }

    if (map->layout[x][y] == FLAME_YELLOW) {
        player->rangeAdd++;
        printf("bomb explosion range + 1\n") ; 
    }

    if (map->layout[x][y] == FLAME_BLUE) {
        player->rangeAdd-- ; 
        printf("bomb explosion range - 1\n") ; 
    }

    if (map->layout[x][y] == FLAME_ROUGE) {
        player->maxRange = true ;
        printf("Max range explosions obtained !\n") ; 
    }

    if (map->layout[x][y] == HEALTH) {
        player->health += 1 ; 
        printf("your health went up ! +20\n") ; 
    }

    if (map->layout[x][y] == INVINCIBILITY) {
        player->invincibility = 2  ;
        printf("You are now invincible for 2 turns you will not be afected explosion \n") ; 
    }

    if (map->layout[x][y] == HEART) {
        player->heart  = true   ;
        printf("Now you will surive the next explosion ! \n") ; 
    }

    if (map->layout[x][y] == BOMB_THROUGH) {
        if (player->bombKick == true) {
            player->bombKick = false ;
            player->passeBomb = true ;
        } else {
            player->passeBomb = true ;
        }
    printf("You have obtained bomb pass ! \n") ; 
    }

    if (map->layout[x][y] == BOMB_KICK) {
        if (player->passeBomb == true) {
            player->passeBomb = false ;
            player->bombKick = true ;
        } else {
            player->bombKick = true ;
        }
    }
    printf("You have obtained bomb kick ! \n") ; 
    
}

void checkIfPlayerInBombRange(Player **playersList , int numberOfPlayers , int x, int y , Bomb *bomb)  
{
    for (int i = 0 ; i < numberOfPlayers ; i++) 
    {
        if (playersList[i]->currentPosition->x == x && playersList[i]->currentPosition->y == y) {
            if (playersList[i]->alive == true && playersList[i]->health > 0)
            {   
                if (playersList[i]->invincibility > 0) {
                    printf("you have surived one explosion %d invincibility left !\n" , playersList[i]->invincibility) ;
                } else 

                if (playersList[i]->health == true) {
                    printf("You have used your heart object to survice one explosion !\n") ; 
                    playersList[i]->heart = false ;
                } else {
                playersList[i]->alive = false ; 
                }
            }
        }   
    }
}

void checkInvincibilityByTurn (Player **playersList , int numberOfPlayers) 
{
    for (int i = 0 ; i < numberOfPlayers ; i++) 
    {
        if (playersList[i]->invincibility > 0) {
            playersList[i]->invincibility-- ; 
        }
    }
}

void clearBombFires (Map *map) 
{
    for (int i= 0 ; i < map->rows ; i++) {
        for (int j = 0 ; j < map->columns ; j++) 
        {
            if (map->layout[i][j] == BOMB_FIRE) {
                map->layout[i][j] = EMPTY_SPACE ; 
            }
        }
    }
} 

// returning bombs to players : checking if the player placed a bomb in the previous trun if yse : player->bombs ++ 
void returnPlayersBombsToInventorys(Player **playersList , int numbreOfPlayers) 
{   
    for (int i = 0 ; i < numbreOfPlayers ; i++ ) 
    {
        if (playersList[i]->plantedAbomb == true) 
        {
            playersList[i]->bombs ++ ; 
        } else {
            continue; 
        }
    }
}

// checking the if there is a winner in the game 
int checkWinner(CurrentGame* currentGame) 
{

    int playersDead = 0 ; 
    for (int i = 0 ; i < currentGame->game->numberOfPlayers ; i++) 
    {
        if (currentGame->game->playerList[i]->alive == false) 
        {
            playersDead++ ;  // counting dead players number in on turn of the game
        }

        // if the number of dead players is equal to the number of players - 1 <one player alive>
        if (playersDead == currentGame->game->numberOfPlayers - 1) 
        {   
            // we search for the living player
            for (int j = 0 ; j < currentGame->game->numberOfPlayers ;j++ ) 
            {
                if (currentGame->game->playerList[i]->alive == true) 
                {   // we print the player name and return true to break the game main loop 
                    printf("Congrats !!! Player %d is the winner of the game\n" , i) ;
                    return true ;  
                }
            } 
        }
        return false;
    }

}

// checking if the player can move to the selected position 
int canMove(Player* player , Map* map , int movement) 
{   
    printf("%c" , movement) ;  

    int selectedRow = player->currentPosition->x ; 
    int selectedCol = player->currentPosition->y  ;
    int result = true ;
    switch (movement) {
    
        case UP :
            selectedRow -= player->rangeAdd ;  // moving down 
            result = cantMoveReason(map , selectedRow , selectedCol) ;
            break;
        case LEFT :        // moving right
            selectedCol -= player->rangeAdd ; 
            result = cantMoveReason(map , selectedRow , selectedCol) ;
            break;
        case RIGHT :        // moving left
            selectedCol += player->rangeAdd ; 
            result = cantMoveReason(map , selectedRow , selectedCol) ;
            break;

        case DOWN :        // moving up
            selectedRow += player->rangeAdd ; 
            result = cantMoveReason(map , selectedRow , selectedCol) ;
            break;

    }
    return result ; 

}
int cantMoveReason(Map * map , int selectedRow , int selectedCol) 
{
    // checking the selected cell wether there is an alredy planted bomb || a wall player can't move 
     if (map->layout[selectedRow][selectedCol] == BOMB) 
        {
            printf(COLOROUTPUT("can't move to that position a bomb is placed there !\n") , 31) ;                 
            return false ; 
            } else 
    // the case there is a wall 
    if (map->layout[selectedRow][selectedCol] == WALL_DESTROYABLE || map->layout[selectedRow][selectedCol] == WALL_UNDESTROYABLE) 
        {
            printf(COLOROUTPUT("can't move player to wanted position a wal exist there !\n") , 31) ;
            return false ;
        } else 
        // in case of going out of the map // 
    if (selectedRow > map->rows || selectedCol > map->columns || selectedRow < 0 || selectedCol < 0 ) 
        {
            printf(COLOROUTPUT("can't move outside the map\n") , 31) ; 
            return false;
    }
    return true ; 
}

void plantBom(Map *map , Player *player , Bomb **activeBombs , CurrentGame *curentGame) 
{   
    // checking if the player's inventory is not empty 
    if (player->bombs == 0) 
    {
        printf(COLOROUTPUT("You have no bombs left in your inventory !\n") , 31) ; 
    } else  
    {      
        // representing the bomb in the map 
        map->layout[player->currentPosition->x][player->currentPosition->y] = BOMB ; 
        // to return the bomb the next turn
        player->plantedAbomb = true ; 
        // taking the bomb from the player inventory 
        player->bombs-- ; 

        //initialising the bomb with player data (position / range)
        Bomb *playerBomb  = malloc(sizeof(Bomb)) ; 
        playerBomb->position = malloc(sizeof(Position)) ;
        playerBomb->position->x = player->currentPosition->x ;
        playerBomb->position->y = player->currentPosition->y ; 

        printf(" bomb position x: %d  | bomb postion y ; %d\n" ,playerBomb->position->x , playerBomb->position->y ) ;
        playerBomb->range = player->rangeAdd ;
        

        // pushing the bomb int the activeBombs array 
        activeBombs[curentGame->numberOfActiveBombs] = playerBomb ;
        curentGame->numberOfActiveBombs++;

        printf("Bomb placed sucssefuly \n") ; 

    }
}
