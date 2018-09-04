/*
 * timerinterrupt.c
 *
 * Created: 10/01/2017 14:13:21
 *  Author: Gustaf Bohlin & Anton Hellbe
 */ 

#include <inttypes.h>
#include "asf.h"
#include "timerinterrupt.h"
#include "../digitalIO/digitalIO.h"
#include "DelayFunctions.h"
#include <pio.h>

/* Define hardware addresses */
#define DACC_ISR 0x400C8030U
#define PIOB_SODR 0x400E1030U
#define PIOB_CODR 0x400E1034U
#define TC_SR4 0x40084060U
#define TC_SR8 0x400880A0U
#define TC_SR0 0x40080020U

/* Declaration of variables */
struct Nibble;
uint32_t outputvalue;
static uint32_t *p_value = &outputvalue;

uint32_t sine[16] = {1768, 2125, 2426, 2628, 2699, 2628, 2426, 2125, 1768, 1412, 1110, 908, 838, 908, 1110, 1412};	//sine wave values
static uint32_t *p_PIOB_SODR = (uint32_t *) PIOB_SODR; // Pointer to PIOB_SODR register
static uint32_t *p_PIOB_CODR = (uint32_t *) PIOB_CODR; // Pointer to PIOB_CODR register
static uint32_t *p_TC_SR4 = (uint32_t*) TC_SR4;		   // Pointer to status register for TC1, channel 1


/* Configure TC1, channel 1 */
void configure_tc4(uint32_t frequency)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();								/* Configure power management of timer clocks */
	
	pmc_enable_periph_clk(ID_TC4);											/* Configure TC for a 44,1kHz frequency and trigger on RC compare. */
	tc_find_mck_divisor(frequency, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC1, 1, ul_tcclks | TC_CMR_CPCTRG);
	
	tc_write_rc(TC1, 1, (ul_sysclk / ul_div) / frequency);						/* Configure and enable interrupt on RC compare */
	NVIC_EnableIRQ((IRQn_Type) TC4_IRQn);		// Enable external interrupt
	tc_enable_interrupt(TC1, 1, TC_IER_CPCS);	// Enable interrupt for TC1, channel 1 on compare.
	tc_start(TC1, 1); //Starts TC1, channel 1
	Nibble.nibb = 0; //Initialize nibble to 0
}

/* Configure TC0, Channel 0 */
void configure_tc0(uint32_t frequency)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();								/* Configure power management of timer clocks */
	
	pmc_enable_periph_clk(ID_TC0);											/* Configure TC for a 44,1kHz frequency and trigger on RC compare. */
	tc_find_mck_divisor(frequency, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
	
	tc_write_rc(TC0, 0, (ul_sysclk / ul_div) / frequency);						/* Configure and enable interrupt on RC compare */
	NVIC_EnableIRQ((IRQn_Type) TC0_IRQn);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
	tc_start(TC0, 0);
}

/* ISR for TC1, Channel 1 */
void TC4_Handler() {
	*p_value = sine[Nibble.nibb++] + ((TRNG->TRNG_ODATA % 297) - 148); // Store sine + calulcated noise in *p_value
	volatile uint32_t dummy = *p_TC_SR4; // Read interrupt flag in status register
	UNUSED(dummy);
}

/* ISR for TC0, Channel 0 */
void TC0_Handler() {
	*p_PIOB_SODR = (0b00000100000000000000000000000000); //Set pin 22 high
	DACC->DACC_CDR = *p_value; // Write to DAC
	(void)(TC0->TC_CHANNEL)->TC_SR; // Read interrupt flag in status register
	*p_PIOB_CODR = (0b00000100000000000000000000000000); // Set pin 22 low
}