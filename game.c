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
	//
	// update player position based on direction of movement
	//

	case LEFT:
		clearPlayer(player);
		player -= 1;
		if (player == 0x7F) {
			player = 0x80;
		}
		if (player == 0xBF) {
			player = 0xC0;
		}
		printPlayer(player);
		waitForP1ButtonRelease(BIT1);
		break;

	case RIGHT:
		clearPlayer(player);
		player += 1;
		if (player == 0x88) {
			player = 0x87;
		}
		printPlayer(player);
		waitForP1ButtonRelease(BIT2);
		break;

	case UP:
		clearPlayer(player);
		if (player >= 0xC0) {
			player -= 0x40;
		}
		printPlayer(player);
		waitForP1ButtonRelease(BIT3);
		break;

	case DOWN:
		clearPlayer(player);
		if (player <= 0x87) {
			player += 0x40;
		}
		printPlayer(player);
		waitForP1ButtonRelease(BIT4);
		break;
	}

	return player;
}

char didPlayerWin(unsigned char player) {
	return player == 0xC7;
}
