/**
 * @file main.c
 *
 * @brief main file initiating all necessary hardware and then blinking LED
 *
 * @author Gustaf Bohlin & Anton Hellbe
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "DelayFunctions.h"
#include "digitalIO/digitalIO.h"
#include "timerinterrupt.h"
#include "da.h"
#include "wdt.h"

#define CONF_BOARD_KEEP_WATCHDOG_AT_INIT 1
#define PIOB_BASE_ADDRESS 0x400E1000U

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).
	sysclk_init();
	board_init();
	delayInit();
	/* Set pins to OUTPUT */
	pinMode(13, OUTPUT);
	pinMode(22, OUTPUT);
	
	/* Calulcate Frequency */
	uint32_t freq = (960721 % 1999) + 991;
	
	/* Configure and start TC0, Channel 0 */
	configure_tc0(freq * 16);
	delayMicroseconds(3600); // Delay to synchronize calculation of noise and DAC output 
	/* Configure and start TC1, Channel 1 */
	configure_tc4(freq * 16);
	
	/* Configure DA */
	configure_da();
	
	/* Runs forever, blink LED every 1,5s and reset watchdogtimer */ 
	while(1) {
		watchDogReload();
		digitalWrite(13, 1);
		delayMicroseconds(750000);
		digitalWrite(13, 0);
		delayMicroseconds(750000);
	}

}
