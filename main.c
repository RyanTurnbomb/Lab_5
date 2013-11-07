#include <msp430.h>
#include "game.h"
#include "button.h"
#include "LCD_Driver.h"
//#include "rand.h"

void init_timer();
void init_buttons();

int TIMER = 0;
int LOSE = 0;
int GAMEOVER = 0;

int main(void) {
	WDTCTL = (WDTPW | WDTHOLD);

	unsigned char player = initPlayer();

	init_timer();
	init_buttons();
	__enable_interrupt();
	initSPI();
	initLCD();

	while (1) {
		player = didPlayerMove(player);
		if (LOSE == 1) {
			LCDclear();
			writeString("GAME");
			setCursorLine2();
			writeString("OVER");
			setCursorLine1();
			GAMEOVER = 1;
			waitForP1ButtonRelease(BIT1 | BIT2 | BIT3 | BIT4);
			debounce();
		}
		if (didPlayerWin(player)) {
			LCDclear();
			writeString("YOU");
			setCursorLine2();
			writeString("WON");
			setCursorLine1();
			GAMEOVER = 1;
			waitForP1ButtonRelease(BIT1 | BIT2 | BIT3 | BIT4);
			debounce();
		}
		if (GAMEOVER) {
			char buttonsToPoll[4] = { BIT1, BIT2, BIT3, BIT4 };
			while (!pollP1Buttons(buttonsToPoll, 4)) {
				//poll until something is pressed
			}
			TAR = 0;
			LOSE = 0;
			TIMER = 0;
			GAMEOVER = 0;
			LCDclear();
			player = initPlayer();
			printPlayer(player);
		}
	}

	return 0;
}

//
// YOUR TIMER A ISR GOES HERE
//

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
	TIMER += 1;
	if (TIMER == 4) {
		LOSE = 1;
	}
}

void init_timer() {
	// do timer initialization work
	TACTL &= ~(MC1 | MC0);        // stop timer

	TACTL |= TACLR;             // clear TAR

	TACTL |= TASSEL1;   // configure for SMCLK - what's the frequency (roughly)?

	TACTL |= ID1 | ID0; // divide clock by 8 - what's the frequency of interrupt?

	TACTL &= ~TAIFG;            // clear interrupt flag

	TACTL |= MC1;               // set count mode to continuous

	TACTL |= TAIE;              // enable interrupt

	__enable_interrupt();       // enable maskable interrupts

}

void init_buttons() {
														// do button initialization work
	configureP1PinAsButton(BIT1 | BIT2 | BIT3 | BIT4);
	P1IE |= BIT1 | BIT2 | BIT3 | BIT4;                  // enable the interrupts
	P1IES |= BIT1 | BIT2 | BIT3 | BIT4; 				// configure interrupt to sense falling edges

}
