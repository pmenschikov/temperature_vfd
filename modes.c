#include <avr/io.h>

#include "modes.h"

#include "lcd.h"

enum mode_t {MAN, AUTO};

enum mode_t current_mode = MAN;

void modes_init()
{
}

void modes_swap()
{
	if( current_mode == MAN )
	{
		current_mode = AUTO;
	}
	else
	{
		current_mode = MAN;
	}

	modes_show();
}

void modes_show()
{
	lcd_set_font(BigFont);
	lcd_set_fgcolor(LCD_BLUE);

	if( current_mode == MAN )
	{
		lcd_print_str_P(PSTR("Manual"), 20, 300);
	}
	else
	{
		lcd_print_str_P(PSTR("  Auto"), 20, 300);
	}
}
