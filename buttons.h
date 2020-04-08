#ifndef BUTTONS_H
#define BUTTONS_H

void buttons_init(void);
void buttons_poll(void);

extern uint8_t buttons_state;
extern uint8_t buttons_changes;

#define BUTTON_10HZ (1<<1)
#define BUTTON_20HZ (1<<2)
#define BUTTON_30HZ (1<<3)
#define BUTTON_40HZ (1<<4)
#define BUTTON_50HZ (1<<5)
#define BUTTON_AUTO (1<<6)

#define IS_BTN_PRESSED(btn)  ((buttons_state&buttons_changes&(btn)) == (btn))
#define IS_BTN_RELEASED(btn) ((~buttons_state&buttons_changes&(btn)) == (btn))


#endif // BUTTONS_H
