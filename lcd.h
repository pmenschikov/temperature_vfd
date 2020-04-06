#ifndef LCD_H
#define LCD_H

// landscape mode
#define LCD_WIDTH   480
#define LCD_HEIGHT  320

#define LCD_BLACK   0x0000
#define LCD_RED     0xF800
#define LCD_GREEN   0x0400
#define LCD_BLUE    0x001F
#define LCD_YELLOW  0xFFE0
#define LCD_WHITE   0xFFFF

void lcd_init(void);
void lcd_clrscr(void);
void lcd_set_fgcolor(uint16_t);
void lcd_set_bgcolor(uint16_t);

void lcd_set_font(const uint8_t *font_ptr);
void lcd_draw_char(char, uint16_t x, uint16_t y);
void lcd_print_str_P(const char*, uint16_t x, uint16_t y);
void lcd_print_str(const char *, uint16_t x, uint16_t y);

extern const uint8_t SmallFont[];
extern const uint8_t BigFont[];
extern const uint8_t big_digits[];


#endif // LCD_H
