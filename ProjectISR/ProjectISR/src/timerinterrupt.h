/*
 * timerinterrupt.h
 *
 * Created: 10/01/2017 14:13:34
 *  Author: Gustaf
 */ 


#ifndef TIMERINTERRUPT_H_
#define TIMERINTERRUPT_H_

void configure_tc4(uint32_t frequency);
void configure_tc0(uint32_t frequency);

struct {
	unsigned int nibb : 4;
} Nibble;

#endif /* TIMERINTERRUPT_H_ */