#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>


#include "lcd.h"
#include "leds.h"

#include "DefaultFonts.cpp"

#define lcd_port  PORTC
#define lcd_ddr   DDRC
#define WR_PIN    _BV(PC0)
#define CS_PIN    _BV(PC1)
#define RS_PIN    _BV(PC2)
#define RST_PIN   _BV(PC3)

#define SET_PIN(pin) (lcd_port |= pin)
#define CLR_PIN(pin) (lcd_port &= ~pin)

#define RS_CLR()     CLR_PIN(RS_PIN)
#define RS_SET()     SET_PIN(RS_PIN)

#define CS_CLR()     CLR_PIN(CS_PIN)
#define CS_SET()     SET_PIN(CS_PIN)

#define pulse_low(pin)  CLR_PIN(pin);SET_PIN(pin);

static void lcd_write(uint8_t hi, uint8_t lo);
static void lcd_cmd(uint8_t cmd);
static void lcd_data8(uint8_t data);
static void lcd_data16(uint16_t data);
static void set_xy(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void delay(uint16_t);

static uint16_t fg_color = LCD_WHITE;
static uint16_t bg_color = LCD_BLACK;

// from UTFT library
struct {
	const uint8_t *ptr;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
} current_font;

void lcd_init()
{
	DDRA = 0xFF; // hi data part
	DDRB = 0xFF; // lo data part

	lcd_ddr |= WR_PIN | CS_PIN | RS_PIN | RST_PIN;

	SET_PIN(RST_PIN);
	delay(5);
	CLR_PIN(RST_PIN);
	delay(15);
	SET_PIN(RST_PIN);
	delay(15);

	lcd_port |= _BV(WR_PIN);

	CS_CLR();

	lcd_cmd(0x11);
	delay(20);
	lcd_cmd(0xD0);
	lcd_data8(0x07);
	lcd_data8(0x42);
	lcd_data8(0x18);

	lcd_cmd(0xD1);
	lcd_data8(0x00);
	lcd_data8(0x07);
	lcd_data8(0x10);

	lcd_cmd(0xD2);
	lcd_data8(0x01);
	lcd_data8(0x02);

	lcd_cmd(0xC0);
	lcd_data8(0x10);
	lcd_data8(0x3B);
	lcd_data8(0x00);
	lcd_data8(0x02);
	lcd_data8(0x11);

	lcd_cmd(0xC5);
	lcd_data8(0x03);

#if 0
	lcd_cmd(0xB6);
	lcd_data8(0x00);
	lcd_data8(0x42);
	lcd_data8(0x3B);
#endif

	lcd_cmd(0xC8);
	lcd_data8(0x00);
	lcd_data8(0x32);
	lcd_data8(0x36);
	lcd_data8(0x45);
	lcd_data8(0x06);
	lcd_data8(0x16);
	lcd_data8(0x37);
	lcd_data8(0x75);
	lcd_data8(0x77);
	lcd_data8(0x54);
	lcd_data8(0x0C);
	lcd_data8(0x00);

	lcd_cmd(0x36);
	lcd_data8(0xEB);

	lcd_cmd(0x3A);
	lcd_data8(0x55);

	lcd_cmd(0x2A);
	lcd_data8(0x00);
	lcd_data8(0x00);
	lcd_data8(0x01);
	lcd_data8(0x3F);

	lcd_cmd(0x2B);
	lcd_data8(0x00);
	lcd_data8(0x00);
	lcd_data8(0x01);
	lcd_data8(0xE0);

	delay(120);

	lcd_cmd(0x29);


	CS_SET();

}

void lcd_clrscr()
{
	uint32_t i=(uint32_t)(LCD_WIDTH)*(uint32_t)(LCD_HEIGHT);
	CS_CLR();

	set_xy(0,0, LCD_WIDTH-1, LCD_HEIGHT-1);

	RS_SET();
	PORTA=0;
	PORTB=0;

	while(i--)
	{
		pulse_low(WR_PIN);
	}

	CS_SET();
}

void lcd_set_fgcolor(uint16_t color)
{
	fg_color = color;
}

void lcd_set_bgcolor(uint16_t color)
{
	bg_color = color;
}

void lcd_set_font(const uint8_t *font_ptr)
{
	current_font.ptr = font_ptr;
	current_font.x_size = pgm_read_byte(&font_ptr[0]);
	current_font.y_size = pgm_read_byte(&font_ptr[1]);
	current_font.offset = pgm_read_byte(&font_ptr[2]);
}

void lcd_draw_char(char ch, uint16_t x, uint16_t y)
{
	uint16_t p;
	uint16_t i;
	int8_t j;
	uint16_t ch_size;
	uint8_t ch_data;

	CS_CLR();

	set_xy(x, y, x+current_font.x_size-1, y+current_font.y_size-1);

	ch_size = (current_font.x_size)*(current_font.y_size)/8;
	p = (ch-current_font.offset)*ch_size +4;

	RS_SET();

	for(i=0; i<ch_size; i++)
	{
		ch_data = pgm_read_byte(current_font.ptr+p);

		for(j=0; j<8; ++j)
		{
			if( (ch_data&(1<<(7-j)))!=0 )
			{
				PORTA = fg_color >> 8;
				PORTB = fg_color;
			}
			else
			{
				PORTA = bg_color >> 8;
				PORTB = bg_color;
			}
			PORTC &= ~1;
			PORTC |= 1;
		}
		p+=1; //(current_font.x_size/8);
	}
	
	set_xy(0,0, LCD_WIDTH-1, LCD_HEIGHT-1);
	CS_SET();
}

void lcd_print_str_P(const char *str, uint16_t x, uint16_t y)
{
	char ch;
	uint16_t offset = x;
	while(ch=pgm_read_byte(str++))
	{
		lcd_draw_char(ch, offset, y);
		offset += current_font.y_size;
	}
}

static void set_xy(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	lcd_cmd(0x2A); // set column address
	lcd_data8(x1>>8);
	lcd_data8(x1);
	lcd_data8(x2>>8);
	lcd_data8(x2);

	lcd_cmd(0x2B); // set page address
	lcd_data8(y1>>8);
	lcd_data8(y1);
	lcd_data8(y2>>8);
	lcd_data8(y2);
	lcd_cmd(0x2C); // write_memory_start
}

static void lcd_cmd(uint8_t cmd)
{
	RS_CLR();
	lcd_write( 0, cmd);
}

static void lcd_data8(uint8_t data)
{
	RS_SET();
	lcd_write( 0, data);
}

static void lcd_data16(uint16_t data)
{
	RS_SET();
	lcd_write( data >> 8, data && 0xFF);
}

static void lcd_write(uint8_t hi, uint8_t lo)
{
	PORTA = hi;
	PORTB = lo;
	pulse_low(WR_PIN);
}
