#ifndef DEBUG_H
#define DEBUG_H

void debug_init(void);
void debug_print_char(char);
void debug_print_str_P(const char *);
void debug_print_str(const char *);
void debug_print_ln(void);
void debug_print_hex_byte(uint8_t);
void debug_print_hex_word(uint16_t);

#endif // DEBUG_H
