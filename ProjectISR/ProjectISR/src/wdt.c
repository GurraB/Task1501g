/*
 * wdt.c
 *
 * Created: 10/01/2017 16:45:38
 *  Author: Gustaf Bohlin & Anton Hellbe
 */ 
#include <inttypes.h>
#include "wdt.h"
#define p_WDT_CR (uint32_t*) 0x400E1A50U

/* Resets watchdog timer */
void watchDogReload() {
	*p_WDT_CR |= (1 << (0)) | (0xA5 << (24));
}