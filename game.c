#include <msp430g2553.h>
#include "game.h"
#include "LCD_Driver.h"

unsigned char initPlayer() {
	return 0x80;
}

void printPlayer(unsigned char PLAYER) {
	writeCommandByte(PLAYER);
	writeDataByte('*');
}

void clearPlayer(unsigned char PLAYER) {
	writeCommandByte(PLAYER);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char PLAYER, unsigned char direction) {
	switch (direction) {
	//
	// update player position based on direction of movement
	//

	case UP:
		PLAYER &= ~BIT6;
		break;

	case DOWN:
		PLAYER |= BIT6;
		break;

	case RIGHT:
		PLAYER++;
		if (PLAYER > 0x87 & PLAYER < 0xC0) {
			PLAYER = 0xC0;
		}
		break;

	case LEFT:
		PLAYER--;
		if (PLAYER < 0x80) {
			PLAYER = 0x80;
		}
		if (PLAYER < 0xC0 & PLAYER > 0x87) {
			PLAYER = 0x87;
		}

	}

	return PLAYER;
}
