#ifndef __MINIGADGET2_H__
#define __MINIGADGET2_H__

// uncomment if you're getting an error related to delay
//#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Some convenient definitions
#define BLUE PA3
#define GREEN PA7
#define RED PB1
#define BUTTON PB2
// May not need these. Vestigial from assn6.
#define READBUTTON PINB & _BV(PB2)
#define SENSORVDD PB0

uint8_t sleep_status;

void initLED(void);
void setLED(uint8_t,uint8_t,uint8_t);
void initButton(void);
void goToSleep(void);

void initSystem(void);
uint8_t median(uint8_t a, uint8_t b, uint8_t c);
uint8_t smallest(uint8_t a, uint8_t b, uint8_t c);
#endif
