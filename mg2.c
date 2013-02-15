#include "mg2.h"

/* For you to do:
   Make sure to declare some globals
   Edit goToSleep
   Edit initLED
   Edit initButton
   Edit the three ISRs */

// You should declare a bunch of volatile LED variables here



void goToSleep(void)
{
	// Delay for button debouncing - we're giving this to you
	_delay_ms(1000);
	// Set the sleep mode (could be done in the initSystem as well - only needs to be done once)
	
	// Set LED outputs to inputs
	
	// Allow stuff to trigger sleep mode but don't go to sleep yet
	
	// Turn off brown-out detect
    

	// Send the sleep instruction
   
		
	// When we get here we've just woken up again, so disable the ability to sleep - brown-out detect automatically comes back
    
	
	// Set LED pins back to ouputs
	
	
	// Delay for a second so that you don't accidentally go to sleep
	_delay_ms(1000);
	// Make it so the button can send us back to sleep (set sleep_status to 0)
	

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
	
	
	// Start pins off active high (which corresponds to LED off)
	
	
	// Initialize timer 0 with a prescalar of 256, then turn on the output compare A and timer overflow interrupts
	
	
}

/*  In this function you should set the button pin to be an input and enable the button pin-change interrupt interrupt */
void initButton()
{
	// Set button as input
	
	// Enable pin change 1 interrupt
	
	// Don't forget to enable PCINT10 in the pin change mask
	
}


// You do not need to modify this function
void initSystem()
{
	sleep_status = 0; // Make it so that we don't start asleep
	initLED();  // Turn on our LED
	initButton(); // Turn on button interrupts
	sei();        // Nothing's gonna interrupt until we turn on global interrupts
}

/* Timer 0 overflow function, should do the follow:
    * Load temporary led values into the regular values
	* Turn on any LED with a non-zero value
	* Find the smallest NON-ZERO LED value
	* Set the output compare register A to that value */
ISR(TIM0_OVF_vect)
{
	
}

/* Timer 0 output compare A ISR, should do the following:
    * Check if an LED value is the same as the output compare, and if so, turn off the corresponding LED
	* Look for the next smallest LED value that is greater than the current output compare value
	* Set the output compare value to the next smallest LED value so we can interrupt again */
ISR(TIM0_COMPA_vect)
{
	

}

/* Pin change interrupt. Set the sleep_status to enable sleep */
ISR(PCINT1_vect)
{
	
}

