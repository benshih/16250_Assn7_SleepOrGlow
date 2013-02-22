#include "mg2.h"

/* For you to do:
   Make sure to declare some globals
   Edit goToSleep
   Edit initLED
   Edit initButton
   Edit the three ISRs */

// You should declare a bunch of volatile LED variables here
volatile uint8_t red_led = 0;
volatile uint8_t blue_led = 0;
volatile uint8_t green_led = 0;
volatile uint8_t red_led_temp = 0;
volatile uint8_t blue_led_temp = 0;
volatile uint8_t green_led_temp = 0;

void goToSleep(void)
{
	// Delay for button debouncing - we're giving this to you
	_delay_ms(1000);
	
    // Set the sleep mode (could be done in the initSystem as well - only needs to be done once)
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// Set LED outputs to inputs
	DDRB &= ~_BV(RED);
    DDRA &= ~_BV(GREEN);
    DDRA &= ~_BV(BLUE);

	// Allow stuff to trigger sleep mode but don't go to sleep yet
    sleep_enable();

	// Turn off brown-out detect. This means that the power is on but low, so
    // the light bulb turns brown.
    sleep_bod_disable();

	// Send the sleep instruction
    sleep_cpu(); // Inline assembly.
		
	// When we get here we've just woken up again, so disable the ability to sleep - brown-out detect automatically comes back
    sleep_disable();
	
	// Set LED pins back to ouputs
    DDRB |= _BV(RED);
    DDRA |= _BV(GREEN);
    DDRA |= _BV(BLUE);
	
	// Delay for a second so that you don't accidentally go to sleep
	_delay_ms(1000);
	// Make it so the button can send us back to sleep (set sleep_status to 0)
	sleep_status = 0;

}

/*  You do not need to modify this function:
    Load the LED values into the temporary variables red_led_temp, green_led_temp, and blue_led_temp */
void setLED(uint8_t red, uint8_t green, uint8_t blue)
{
	red_led_temp = red;
	green_led_temp = green;
	blue_led_temp = blue;
}

/* In this function you should set the LED pins to be outputs and
   set them high to turn them off (because our LEDs are active low)
   Lastly, you should initialize timer 0's interrupts and prescalar
   to enable software PWM  */
void initLED()
{
	// Set LED pin direction to output
	DDRB |= _BV(RED);
    DDRA |= _BV(GREEN);
    DDRA |= _BV(BLUE);
	
	// Start pins off active high (which corresponds to LED off)
	PORTB |= _BV(RED);
    PORTA |= _BV(GREEN);
    PORTA |= _BV(BLUE);
	
	// Initialize timer 0 with a prescalar of 256, then turn on the output compare A and timer overflow interrupts
    TCCR0B |= _BV(CS02);
    TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0);
    
    // WDTCSR = _BV(WDP2) | _BV(WDP1) | _BV(WDP0); // Resets everything for me after a certain period of time just in case I mess up the chip.
	
}

/*  In this function you should set the button pin to be an input and enable the button pin-change interrupt interrupt */
void initButton()
{
	// Set button as input
    DDRB &= ~_BV(BUTTON);
	
	// Enable pin change 1 interrupt
    GIMSK |= _BV(PCIE1); // Pin Change Interrupt Enable 1.

	// Don't forget to enable PCINT10 in the pin change mask
	PCMSK1 |= _BV(PCINT10); // Pin Change Interupt 10.
}


// You do not need to modify this function
void initSystem()
{
	sleep_status = 0; // Make it so that we don't start asleep
	initLED();  // Turn on our LED
	initButton(); // Turn on button interrupts
	sei();        // Nothing's gonna interrupt until we turn on global interrupts
}

// Given two ints, return the smaller nonzero one. If either or both are
// non-positive, automatically return the larger positive one or 0 if both are non-positive.
uint8_t smallerNonZero(uint8_t one, uint8_t two)
{
    if(0 >= one)
    {
        return (two>0?two:0);
    }
    if(0 >= two)
    {
        return (one>0?one:0);
    }
    if(one > two)
    {
        return one;
    }
    else
    {
        return two;
    }
}

/* Timer 0 overflow function, should do the follow:
    * Load temporary led values into the regular values
	* Turn on any LED with a non-zero value
	* Find the smallest NON-ZERO LED value
	* Set the output compare register A to that value */
ISR(TIM0_OVF_vect)
{
	// Load temporary led values into the regular values.
    red_led = red_led_temp;
    blue_led = blue_led_temp;
    green_led = green_led_temp;

    // Turn on any LED with a non-zero value.
    if(0 != red_led)
    {
        PORTB &= ~_BV(RED);
    }
    else
    {
        PORTB |= _BV(RED);
    }
    if(0 != blue_led)
    {
        PORTA &= ~_BV(BLUE);
    }
    else
    {
        PORTA |= _BV(BLUE);
    }
    if(0 != green_led)
    {
        PORTA &= ~_BV(GREEN);
    }
    else
    {
        PORTA |= _BV(GREEN);
    }

    // Find the smallest non-zero LED value.

    int tempMin = smallerNonZero(red_led, blue_led);
    int min = smallerNonZero(tempMin, green_led);
    
    // Set the output compare register A to that value.
    OCR0A = min;
}

/* Timer 0 output compare A ISR, should do the following:
    * Check if an LED value is the same as the output compare, and if so, turn off the corresponding LED
	* Look for the next smallest LED value that is greater than the current output compare value
	* Set the output compare value to the next smallest LED value so we can interrupt again */
ISR(TIM0_COMPA_vect)
{
    // Check if an LED value is the same as the output compare, and if so,
    // turn off the corresponding LED.
	if(OCR0A == red_led)
    {
        PORTB |= _BV(RED);
    }
    if(OCR0A == blue_led)
    {
        PORTA |= _BV(BLUE);
    }
    if(OCR0A == green_led)
    {
        PORTA |= _BV(GREEN);
    }

    // Look for the next smallest LED value that is greater than the current
    // output compare value.
    int temp_red = red_led - OCR0A;
    int temp_blue = blue_led - OCR0A;
    int temp_green = green_led - OCR0A;

    int temp_min = smallerNonZero(temp_red, temp_blue);
    int min = smallerNonZero(temp_min, temp_green);
    
    int nextSmallestLED = min + OCR0A;

    // Set the output compare value to the next smallest LED value so we can
    // interrupt again.
    OCR0A = nextSmallestLED;
}

/* Pin change interrupt. Set the sleep_status to enable sleep */
ISR(PCINT1_vect)
{
    sleep_status = 1;
}

