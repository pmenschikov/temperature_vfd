#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"

static void init(void)
{
	LED_INIT();
}

int main()
{
	init();

	while(1)
	{
		LED_TOGGLE(LED_ALL);
		_delay_ms(100);
	}
	return 0;
}
