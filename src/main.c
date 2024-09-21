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

#define VGA_COLOR(FG, BG) ((FG) | ((BG) << 4))
#define VGA_CHAR(CHAR, COL) (((uint16_t)CHAR) | (((uint16_t)COL) << 8))

static unsigned vga_ptr = 0;

void putchar(uint8_t c)
{
    ((uint16_t *)VGA_BUFFER)[vga_ptr++] = VGA_CHAR(c, VGA_COLOR(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLUE));
}

int puts(const char *str)
{
    int n = 0;
    for (char *ptr = (char *)str; *ptr; ++ptr, ++n)
        putchar(*ptr);
    return n;
}

void main(void)
{
    for (vga_ptr = 0; vga_ptr < VGA_WIDTH * VGA_HEIGHT; putchar(' '))
        ;

    vga_ptr = 0;
    puts("Hello World!");
}
