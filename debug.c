#include <avr/io.h>
#include <avr/pgmspace.h>

#include "debug.h"

#define BAUD 500000UL

void debug_init()
{
#include <util/setbaud.h>
	UBRR1L = UBRRL_VALUE;
	UBRR1H = UBRRH_VALUE;
		
	UCSR1B = _BV(TXEN);
	UCSR1C = (1<<USBS)|(3<<UCSZ0);

	DDRD |= _BV(PD4); // RTS singal
	PORTD |= _BV(PD4);
}

void debug_print_char(char ch)
{
	while( !( UCSR1A & _BV(UDRE) ) );
	UDR1 = ch;
}

// print string from flash memory
void debug_print_str_P(const char *s)
{
	char ch;
	ch = pgm_read_byte(s++);
	while(ch)
	{
		debug_print_char(ch);
		ch = pgm_read_byte(s++);
	}
}

void debug_print_str(const char *s)
{
	while(*s)
	{
		debug_print_char(*s);
		s++;
	}
}

void debug_print_ln()
{
	debug_print_char('\r');
	debug_print_char('\n');
}

static uint8_t h2b(uint8_t v)
{
	return v > 9 ? v-10+'A':v+'0';
}

void debug_print_hex_byte(uint8_t b)
{
	debug_print_char(h2b(b>>4));
	debug_print_char(h2b(b&0xF));
}

void debug_print_hex_word(uint16_t w)
{
	debug_print_hex_byte(w>>8);
	debug_print_hex_byte(w&0xFF);
}
