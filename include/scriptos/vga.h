#pragma once

#include <stdbool.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER 0xB8000

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

#define VGA_POS(X, Y) ((X) + (Y) * VGA_WIDTH)
#define VGA_COLOR(FG, BG) ((FG) | ((BG) << 4))
#define VGA_CHAR(CHAR, COL) (((uint16_t)CHAR) | (((uint16_t)COL) << 8))

#define PRINTF_STATE_NONE 0
#define PRINTF_STATE_FLAGS 1
#define PRINTF_STATE_WIDTH 2
#define PRINTF_STATE_PRECISION 3
#define PRINTF_STATE_LENGTH 4
#define PRINTF_STATE_SPECIFIER 5

#define PRINTF_FLAG_LEFT_JUSTIFY 0b00001
#define PRINTF_FLAG_FORCE_SIGN 0b00010
#define PRINTF_FLAG_INSERT_BLANK 0b00100
#define PRINTF_FLAG_INSERT_PREFIX 0b01000
#define PRINTF_FLAG_LEFT_PAD 0b10000

extern uint16_t vga_position;
extern uint8_t vga_color;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor(void);
void update_cursor(uint16_t pos);
void update_cursor_xy(int x, int y);
uint16_t get_cursor_position(void);

void set_color(uint8_t fg, uint8_t bg);
void clear_screen();
void putc(int c);
int puts(const char *str);

void mem_dump(const void *begin, const void *end);

void put(int c);
int putint(int value, unsigned base, int flags, int width, int precision, int length, bool is_signed, bool uppercase);
int putfloat(double value, int flags, int width, int precision, int length);
int printf(const char *format, ...);
