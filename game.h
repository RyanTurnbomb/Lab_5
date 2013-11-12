/*
 * game.h
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Ryan.Turner
 */

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define ROW_MASK 0x40

#define NUM_MINES 2

//
// Initializes player to starting position on board.
//
unsigned char initPlayer();

//
// Prints the player marker (*) at the player location passed in.
//
void printPlayer(unsigned char PLAYER);

//
// Clears the location passed in.
//
void clearPlayer(unsigned char PLAYER);

//
// Given a player's current position and a direction, returns an updated player position.
//
unsigned char movePlayer(unsigned char PLAYER, unsigned char direction);

//
// Returns true if the position passed in is the winning game position.
//
unsigned char didPlayerMove(unsigned char PLAYER);

//
// Checks if player moved
//
char didPlayerWin(unsigned char PLAYER);

/*
 * Functions which may prove useful in achieving A-Functionality.
 *
char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
void generateMines(unsigned char mines[NUM_MINES]);
void printMines(unsigned char mines[NUM_MINES]);
*/
