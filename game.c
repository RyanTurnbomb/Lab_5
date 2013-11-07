#include <msp430g2553.h>
#include "game.h"
#include "LCD_Driver.h"

unsigned char initPlayer() {
	return 0x80;
}

void printPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction) {
	switch (direction) {
	case LEFT:
		clearPlayer(player);

		printPlayer(player);
		waitForP1ButtonRelease(BIT1);
		break;

	case RIGHT:
		clearPlayer(player);

		printPlayer(player);
		waitForP1ButtonRelease(BIT2);
		break;

	case UP:
		clearPlayer(player);

		printPlayer(player);
		waitForP1ButtonRelease(BIT3);
		break;

	case DOWN:
		clearPlayer(player);

		printPlayer(player);
		waitForP1ButtonRelease(BIT4);
		break;
		//
		// update player position based on direction of movement
		//
	}

	return player;
}

char didPlayerWin(unsigned char player) {
	return player == 0xC7;
}
