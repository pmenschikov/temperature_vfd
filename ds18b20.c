
#include "ds18b20.h"

uint8_t ds_start()
{
	if(!onewire_init())
	{
		return 0;
	}
	onewire_command(ONEWIRE_SKIP_ROM);
	onewire_command(DS18X20_CONVERT_T);
	return 1;
}

uint8_t ds_readtemp(int16_t *temp)
{
	if(!onewire_init())
	{
		return 0;
	}
	onewire_command(ONEWIRE_SKIP_ROM);
	onewire_command(DS18X20_RD_SCRTCHPD);
	*temp = onewire_recvbyte();
	*temp |= onewire_recvbyte()<<8;

	return 1;
}
