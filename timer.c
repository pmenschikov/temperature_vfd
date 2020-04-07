#include "timer.h"

#include "debug.h"

#define MAX_TIMERS 8

struct timer_t
{
	uint8_t timer;
	void (*callback)();
};

struct timer_t timers[MAX_TIMERS];

void timer_init()
{
	uint8_t i;
	for(i=0; i<MAX_TIMERS; ++i)
	{
		timers[i].timer = 0;
		timers[i].callback = 0;
	}

	OCR1A = 0x3D06; // 1sec interval
	TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10); // CTC mode
}

void timer_poll()
{
	uint8_t i;

	if( TIFR&_BV(OCF1A) )
	{
		TIFR |= _BV(OCF1A); // clear bit

		for(i=0; i<MAX_TIMERS; ++i)
		{
			if( timers[i].timer > 0 )
			{
				timers[i].timer--;

				if( timers[i].timer == 0 && timers[i].callback )
					timers[i].callback();
			}
		}
	}
}

void timer_start(uint8_t id, uint8_t timeout)
{
	timers[id].timer = timeout;
}

uint8_t timer_elapsed(uint8_t id)
{
	return timers[id].timer == 0;
}

void timer_set_callback(uint8_t id, void (*cb)())
{
	timers[id].callback = cb;
}
