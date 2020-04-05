#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "leds.h"
#include "lcd.h"

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


	welcome_screen();


	LED_ON(LED_GREEN);

	lcd_clrscr();
	lcd_set_fgcolor(LCD_GREEN);
	lcd_print_str_P(PSTR("Ready"), 0, 0);

	while(1)
	{
		LED_TOGGLE(LED_GREEN);
		delay(500);
	}
	return 0;
}
