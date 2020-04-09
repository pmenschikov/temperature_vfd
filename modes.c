#include <avr/io.h>
#include <stdio.h>

#include "modes.h"

#include "lcd.h"

enum mode_t {MAN, AUTO};

enum mode_t current_mode = MAN;

static uint8_t manual_freq = 10;

void modes_init()
{
}

static void show_man(void);
static void show_auto(void);

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
		show_man();
	}
	else
	{
		show_auto();
	}
}

void modes_man_set_freq(uint8_t freq)
{
	manual_freq = freq;
	modes_show();
}

static void show_man()
{
	char buffer[8];
	lcd_print_str_P(PSTR("Manual"), 20, 300);

	sprintf_P(buffer, PSTR("%d Hz"), manual_freq);
	lcd_print_str( buffer, 20+16*6+16, 300);
}

static void show_auto()
{
	char buffer[8];

	lcd_print_str_P(PSTR("  Auto"), 20, 300);

	sprintf_P(buffer, PSTR("%d%%   "), 44);
	lcd_print_str(buffer, 20+16*6+16, 300);
}
