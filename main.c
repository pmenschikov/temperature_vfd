#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <stdio.h>

#include "leds.h"
#include "lcd.h"
#include "debug.h"
#include "ds18b20.h"
#include "timer.h"
#include "termometer.h"
#include "shared.h"
#include "values.h"
#include "buttons.h"

void delay(uint16_t ms)
{
	while(ms--)
	{
		_delay_ms(1);
	}
}

static void init(void)
{
	LED_INIT();

	lcd_init();

	debug_init();

	timer_init();

	buttons_init();

}

static void welcome_screen(void)
{
	lcd_clrscr();

	lcd_set_font(BigFont);

	lcd_set_fgcolor(LCD_RED);
	lcd_print_str_P(PSTR("VFD Temperature controller"), 0,0 );

	lcd_set_font(SmallFont);

	delay(2000);

	lcd_clrscr();
}


static void timer1_cb(void)
{
	char buffer[4];

	timer_start(1, 1);
	LED_TOGGLE(LED_GREEN);

	// show temperature
	if( temp_sensor_status == 0 )
	{

		sprintf(buffer, "%d", temperature/16);

		lcd_print_str(buffer, 160, 20);
	}
	else
	{
		lcd_print_str_P(PSTR("No DS"), 160, 60);
	}
}


int main()
{
	uint8_t btns = 0;
	init();

	debug_print_str_P(PSTR("Init done\r\n"));

	welcome_screen();

	LED_ON(LED_GREEN);

	lcd_clrscr();
	lcd_set_fgcolor(LCD_GREEN);
	lcd_print_str_P(PSTR("Ready"), 0, 0);

	debug_print_str_P(PSTR("Started\r\n"));

	lcd_set_font(big_digits);
	lcd_set_fgcolor(LCD_GREEN);

	lcd_clrscr();

	timer_start(TIMER_TERMO, 1);
	timer_set_callback(TIMER_TERMO, termometer_task);

	// screen update
	timer_start(1, 2);
	timer_set_callback(1, timer1_cb);

	sei();

	while(1)
	{
		timer_poll();

		buttons_poll();

		if( IS_BTN_PRESSED(BUTTON_AUTO) )
		{
			debug_print_str_P(PSTR("Btn AUTO pressed\r\n"));
		}

		if( IS_BTN_RELEASED(BUTTON_AUTO) )
		{
			debug_print_str_P(PSTR("Btn AUTO released\r\n"));
		}
	}
	return 0;
}
