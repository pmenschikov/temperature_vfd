#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "leds.h"
#include "lcd.h"
#include "debug.h"

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
	lcd_print_str_P(PSTR("22"), 0, 20);

	while(1)
	{
		LED_TOGGLE(LED_GREEN);
		delay(500);
		debug_print_char('*');
	}
	return 0;
}
