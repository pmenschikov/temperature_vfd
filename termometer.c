#include <avr/io.h>

#include "termometer.h"

#include "onewire.h"
#include "ds18b20.h"
#include "timer.h"
#include "shared.h"
#include "values.h"

#define MEASURE_INTERVAL 1

static void read_temperature(void);

void termometer_task()
{
	if( ds_start() )
	{
		timer_start(TIMER_TERMO, 1);
		timer_set_callback(TIMER_TERMO, read_temperature);
	}
	else
	{
		timer_start(TIMER_TERMO, MEASURE_INTERVAL);
		temp_sensor_status = 1;
	}
}

static void read_temperature()
{
	if( ds_readtemp(&temperature) )
	{
		temp_sensor_status = 0;
	}

	timer_start(TIMER_TERMO, MEASURE_INTERVAL);
	timer_set_callback(TIMER_TERMO, termometer_task);
}
