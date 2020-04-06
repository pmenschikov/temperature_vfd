
/*
 * small 1-wire lib for DS18B20 
 */

#include <avr/io.h>
#include <util/delay.h>

#include "onewire.h"

#define DS18X20_ALARM_SEARCH	(0xEC)
#define DS18X20_CONVERT_T	(0x44)
#define DS18X20_RD_SCRTCHPD	(0xBE)
#define DS18X20_WR_SCRTCHPD	(0x4E)
#define DS18X20_CP_SCRTCHPD	(0x48)
#define DS18X20_RECALL_E		(0xB8)
#define DS18X20_RD_PWR_SPL	(0xB4)

uint8_t ds_start();
uint8_t ds_readtemp(int16_t *temp);
