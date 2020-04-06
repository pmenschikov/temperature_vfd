/*
 * small 1-wire lib for DS18B20 
 */

#include <avr/io.h>
#include <util/delay.h>

#define ONEWIRE_DDR     DDRF
#define ONEWIRE_PORT    PORTF
#define ONEWIRE_PINS    PINF
#define ONEWIRE_PIN     PINF0

#define ONEWIRE_SEARCH_ROM (0xF0)
#define ONEWIRE_READ_ROM   (0x33)
#define ONEWIRE_MATCH_ROM  (0x55)
#define ONEWIRE_SKIP_ROM   (0xCC)


uint8_t onewire_init(void);
void onewire_command( unsigned char cmd );
uint8_t onewire_recvbyte(void);
unsigned int read_temp(void);
