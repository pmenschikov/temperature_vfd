#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdio.h>

#include "leds.h"
#include "lcd.h"
#include "debug.h"
#include "ds18b20.h"

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

static void read_temperature(void)
{
	int16_t temperature;
	char buffer[4];

	LED_OFF(LED_RED);

	if( ds_start() )
	{
		// DS18B20 found
		delay(700);
		ds_readtemp(&temperature);

		temperature /= 16;

		sprintf(buffer, "%d", temperature);

		lcd_print_str(buffer, 160, 20);
	}
	else
	{
		LED_ON(LED_RED);
		debug_print_str_P(PSTR("NoDS\r\n"));
		lcd_clrscr();
		lcd_print_str_P(PSTR("NoDS"), 180, 20);
		// DS not found
	}

}

int main()
{
	int i,j;

	init();

	debug_print_str_P(PSTR("Init done\r\n"));

	welcome_screen();

	LED_ON(LED_GREEN);

	lcd_clrscr();
	lcd_set_fgcolor(LCD_GREEN);
	lcd_print_str_P(PSTR("Ready"), 0, 0);

	debug_print_str_P(PSTR("Started\r\n"));

	lcd_set_font(big_digits);
	lcd_set_fgcolor(LCD_RED);

	lcd_clrscr();

	while(1)
	{
		read_temperature();
		LED_TOGGLE(LED_GREEN);
		delay(1000);
		debug_print_char('*');
	}
	return 0;
}
