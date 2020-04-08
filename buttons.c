#include <avr/io.h>
#include <avr/interrupt.h>

#include "buttons.h"

#define BUTTON_MASK 0x7E

static volatile uint8_t btn_state;
uint8_t buttons_state;
uint8_t buttons_prev_state = 0;
uint8_t buttons_changes = 0;

void buttons_init()
{
	PORTF |= BUTTON_MASK; // buttons pull-ups resitors

	// TMR0 - CTC mode, prescaller - 256, ovf rate ~4ms
	TCCR0 = _BV(WGM01) | _BV(CS01) | _BV(CS02) | _BV(CS00);

	TIMSK |= _BV(OCIE0);
}

// Using vertial counters
// https://www.compuphase.com/electronics/debouncing.htm
ISR(TIMER0_COMP_vect)
{
	static uint8_t cnt1, cnt0;
	uint8_t delta;

	delta= buttons_state ^ ((~PINF) & BUTTON_MASK);

	cnt1 = (cnt1 ^ cnt0) & delta;
	cnt0 = ~cnt0 & delta;

	btn_state ^= (cnt0 & cnt1);
}

void buttons_poll()
{
	buttons_state = btn_state;

	buttons_changes = buttons_state ^ buttons_prev_state;
	buttons_prev_state = buttons_state;
}
