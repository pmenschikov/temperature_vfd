/*
 * small 1-wire lib for DS18B20 
 */

#include <avr/interrupt.h>
#include "onewire.h"

static void delay_us(uint16_t us)
{
	while(us--)
	{
		_delay_us(1);
	}
}

// return 1 if any device on bus
uint8_t onewire_init()
{
	uint8_t ret;
	ONEWIRE_DDR |= _BV(ONEWIRE_PIN);
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN);
	delay_us(480);
	ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN);
	cli();
	delay_us(60);
	ret = ONEWIRE_PINS&_BV(ONEWIRE_PIN) ? 0:1;
	sei();
	delay_us(420);
	return ret;
}

void onewire_command( unsigned char cmd )
{
	unsigned char i;
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN);
	for( i=0; i<8; i++ )
	{
		cli();
		ONEWIRE_DDR |= _BV(ONEWIRE_PIN);
		delay_us(1);
		if( cmd & 0x01 )
		{
			ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN);
		}
		sei();
		delay_us(60);
		ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN);
		cmd >>= 1;
	}
}

unsigned char onewire_recvbyte()
{
	unsigned char i;
	unsigned char ret = 0;
	for( i=0; i<8; i++ )
	{
		cli();
		ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN);
		ONEWIRE_DDR |= _BV(ONEWIRE_PIN);
		delay_us(1);
		ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN);
		delay_us(14);
		if( ONEWIRE_PINS & _BV(ONEWIRE_PIN) )
		{
			ret |= 0x80;
		}
		sei();
		delay_us(420);
		if(i!=7)
		ret>>=1;
	}
	return ret;
}

static unsigned char onewire_read_bit(void)
{
	unsigned char ret = 0;
	cli();
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PIN);
	ONEWIRE_DDR |= _BV(ONEWIRE_PIN);
	delay_us(1);
	ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN);
	delay_us(14);
	if( ONEWIRE_PINS & _BV(ONEWIRE_PIN) )
	{
		ret = 0x80;
	}
	sei();
	delay_us(420);
	return ret;
}

