#ifndef LEDS_H
#define LEDS_H

#include <avr/io.h>


#define LED_DDR         DDRD
#define LED_PORT        PORTD
#define LED_RED         _BV(PD5)
#define LED_YELLOW      _BV(PD6)
#define LED_GREEN       _BV(PD7)

#define LED_ALL (LED_RED | LED_YELLOW | LED_GREEN)

#define LED_TOGGLE(led) LED_PORT ^= (led)
#define LED_ON(led)     LED_PORT |= (led)
#define LED_OFF(led)    LED_PORT &= ~(led)

#define LED_INIT() LED_DDR |= LED_ALL;

#endif // LEDS_H
