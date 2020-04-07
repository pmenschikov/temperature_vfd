#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void timer_init(void);
void timer_poll(void);
void timer_start(uint8_t id, uint8_t timeout);
uint8_t timer_elapsed(uint8_t id);
void timer_set_callback(uint8_t id, void (*)(void));

#endif // TIMER_H
