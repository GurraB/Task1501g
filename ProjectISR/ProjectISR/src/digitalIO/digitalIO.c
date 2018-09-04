/*
 * digitalIO.c
 *
 * Created: 2015-06-10 15:11:18
 *  Author: Gustaf Bohlin & Anton Hellbe
 */ 

#include <inttypes.h>	/* See http://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types for more info */
#include <asf.h>		/* Only needed to get the definitions for HIGH and LOW */
#include "digitalIO.h"

#define PIOB_BASE_ADDRESS 0x400E1000U



void pinMode(int pinNumber, mode_definition mode)
{
	uint32_t *const p_PIOB_OER = (uint32_t *) (PIOB_BASE_ADDRESS+0x0010U);	/* defines the address for enabling the parallel output B register */
	uint32_t *const p_PIOB_PER = (uint32_t *) (PIOB_BASE_ADDRESS+0x0000U);	/* defines the address for enabling the parallel Input/output B register */
	
	if(pinNumber == 13) {
		pinNumber = 27;
	}else {
		pinNumber = 26;
	}

	if (mode == OUTPUT)	/* You only have to program a function that cares about OUTPUT, and does nothing for the other values */
	{
		*p_PIOB_PER |= (1<<(pinNumber));
		*p_PIOB_OER |= (1<<(pinNumber));
	}
	else
	{
		/* Do nothing */
	}
	
}

void digitalWrite(int pinNumber, int value)
{
	uint32_t *const p_PIOB_SODR = (uint32_t *) (PIOB_BASE_ADDRESS+0x0030U);	/* defines the address for setting the output pins of the B register */
	uint32_t *const p_PIOB_CODR = (uint32_t *) (PIOB_BASE_ADDRESS+0x0034U);	/* defines the address for setting the output pins of the B register */
	
	if(pinNumber == 13) {
		pinNumber = 27;
	}
	else {
		pinNumber = 26;
	}
	
	
	if (value == HIGH)
	{
		*p_PIOB_SODR |= (1<<(pinNumber));
	}
	else if (value == LOW)
	{
		*p_PIOB_CODR |= (1<<(pinNumber));
	}
	else
	{
		/* Something is wrong */
	}
}