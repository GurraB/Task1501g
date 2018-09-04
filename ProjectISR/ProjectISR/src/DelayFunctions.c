/**
 * @file DelayFunctions.c
 *
 * @brief Code for a simple delay using TC0
 * The timer in the present code cannot be the same timer
 * as is used for the interrupt code (TCx_handler).
 *
 * @author Uek
 *
 * @date 2015
 */

#include "asf.h"
#include "DelayFunctions.h"

void delayInit(void)		/* Initializes the timer used for delays */
{
	pmc_enable_periph_clk(ID_TC8);
	tc_init(TC2,2,0);		 /* TC0, channel 0, TCLK1 och capturemode */
	tc_set_block_mode(TC2,0);
	tc_stop(TC2,2);			/* making sure the timer does not run  */
}


void delayMicroseconds(uint32_t us)		/* A simple implementation for a delay in us (not calibrated) */
{
	tc_start(TC2,2);
	while (tc_read_cv(TC2,2) < us*42); /* Only works in newere version of ASF */
	tc_stop(TC2,2);
}