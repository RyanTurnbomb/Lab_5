#include <msp430.h>
#include "game.h"
#include "button.h"
#include "LCD_Driver.h"
//#include "rand.h"

void init_timer();
void init_buttons();
void TestButton(char buttonToTest);
void movecursor(char buttonToTest);
void newGame();
void Reset(char buttonToTest);

char PLAYER = 0;
char TIMER = 0;
char DONE = 0;
char buttonToTest = 0;

void ClearTimer() {
	TACTL |= TACLR;
	TIMER = 0;
}

int main(void) {
	WDTCTL = (WDTPW | WDTHOLD);

	PLAYER = initPlayer();

	initSPI();
	initLCD();
	LCDclear();
	newGame();
	printPlayer(PLAYER);
	init_timer();
	init_buttons();
	__enable_interrupt();

	while (1) {
		if (PLAYER == 0xC7) {

			TACTL &= ~TAIE;
			LCDclear();
			setCursorLine1();
			writeString("YOU");
			setCursorLine2();
			writeString("WON!");
			DONE = 1;
			_delay_cycles(1000000);

		}

		if (TIMER >= 4) {
			TACTL &= ~TAIE;
			LCDclear();
			setCursorLine1();
			writeString("GAME");
			setCursorLine2();
			writeString("OVER");
			DONE = 1;
			_delay_cycles(1000000);

		}

	}

	return 0;
}

//
// YOUR TIMER A ISR GOES HERE
//

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;
	TIMER++;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
	if (DONE == 0) {
		TestButton(BIT1);
		TestButton(BIT2);
		TestButton(BIT3);
		TestButton(BIT4);
	} else {
		Reset(BIT1);
		Reset(BIT2);
		Reset(BIT3);
		Reset(BIT4);
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
	P1DIR &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P1IE |= BIT1 | BIT2 | BIT3 | BIT4;
	P1IES |= BIT1 | BIT2 | BIT3 | BIT4;
	P1REN |= BIT1 | BIT2 | BIT3 | BIT4;
	P1OUT |= BIT1 | BIT2 | BIT3 | BIT4;
	P1IFG &= ~(BIT1 | BIT2 | BIT3 | BIT4);
}

void TestButton(char buttonToTest) {
	if (buttonToTest & P1IFG) {
		if (buttonToTest & P1IES) {
			movecursor(buttonToTest);
			ClearTimer();
		}
		else {
			debounce();
		}

		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}
}

void movecursor(char buttonToTest) {
	clearPlayer(PLAYER);
	switch (buttonToTest) {
	case BIT1:
		PLAYER = movePlayer(PLAYER, RIGHT);
		break;
	case BIT2:
		PLAYER = movePlayer(PLAYER, LEFT);
		break;
	case BIT3:
		PLAYER = movePlayer(PLAYER, UP);
		break;
	case BIT4:
		PLAYER = movePlayer(PLAYER, DOWN);
		break;
	}
	printPlayer(PLAYER);
}

void newGame() {
	DONE = 0;
	LCDclear();
	PLAYER = initPlayer();
	printPlayer(PLAYER);

}

void Reset(char buttonToTest) {
	if (buttonToTest & P1IFG) {
		if (buttonToTest & P1IES) {
			newGame();
			ClearTimer();
			TACTL |= TAIE;
		}
		else {
			debounce();
		}

		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}
}
