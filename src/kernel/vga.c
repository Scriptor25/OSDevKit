#include <scriptos/io.h>
#include <scriptos/vga.h>

uint16_t vga_position = 0;
uint8_t vga_color = VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void update_cursor(uint16_t pos)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

    vga_position = pos;
}

void update_cursor_xy(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;
    update_cursor(pos);
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void set_color(uint8_t fg, uint8_t bg)
{
    vga_color = VGA_COLOR(fg, bg);
}

void clear_screen()
{
    uint16_t *buffer = (uint16_t *)VGA_BUFFER;
    for (unsigned i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        *(buffer++) = VGA_CHAR(' ', vga_color);
}

void putc(int c)
{
    put(c);
    update_cursor(vga_position);
}

int puts(const char *str)
{
    int n = 0;
    uint16_t *buffer = &((uint16_t *)VGA_BUFFER)[vga_position];
    for (char *ptr = (char *)str; *ptr; ++n)
        *(buffer++) = VGA_CHAR(*(ptr++), vga_color);

    vga_position += n;
    int y = vga_position / VGA_WIDTH;
    update_cursor_xy(0, y + 1);
    return n;
}

#define MEM_DUMP_WIDTH 16

void mem_dump(const void *begin, const void *end)
{
    char *p;
    unsigned i;
    for (p = (char *)begin; p < (char *)end; p += MEM_DUMP_WIDTH)
    {
        printf("%08p | ", p);
        for (i = 0; i < MEM_DUMP_WIDTH; ++i)
            printf("%02X ", (unsigned char)p[i]);
        printf("| ");
        for (i = 0; i < MEM_DUMP_WIDTH; ++i)
            printf("%c", (p[i] >= 0x20) ? p[i] : '.');
        printf("\n");
    }
}

void put(int c)
{
    if (vga_position >= (VGA_WIDTH * VGA_HEIGHT))
        vga_position = 0;
    ((uint16_t *)VGA_BUFFER)[vga_position++] = VGA_CHAR(c, vga_color);
}

int putint(int value, unsigned base, int flags, int width, int precision, int length, bool is_signed, bool uppercase)
{
    (void)precision;
    (void)length;

    char buffer[256];
    unsigned i = 0, j;
    bool has_sign = false;

    if (is_signed && value < 0)
    {
        value = -value;
        has_sign = true;
    }

    uint16_t begin = vga_position;

    do
    {
        unsigned rem = value % base;
        value /= base;
        buffer[i++] = (rem < 10 ? (rem + 0x30) : (uppercase ? (rem + 0x37) : (rem + 0x57)));
    } while (value > 0);

    if (flags & PRINTF_FLAG_INSERT_PREFIX)
    {
        if (base == 8)
            buffer[i++] = '0';
        else if (base == 16)
        {
            buffer[i++] = (uppercase ? 'X' : 'x');
            buffer[i++] = '0';
        }
    }

    if (flags & PRINTF_FLAG_FORCE_SIGN)
    {
        if (!has_sign)
            buffer[i++] = '+';
    }
    else if (flags & PRINTF_FLAG_INSERT_BLANK)
    {
        if (!has_sign)
            buffer[i++] = ' ';
    }

    if (has_sign)
        buffer[i++] = '-';

    if (!(flags & PRINTF_FLAG_LEFT_JUSTIFY))
    {
        for (j = i; j < (unsigned)width; ++j)
            if ((flags & PRINTF_FLAG_LEFT_PAD))
                put('0');
            else
                put(' ');
    }

    for (j = i; j > 0; --j)
        put(buffer[j - 1]);

    update_cursor(vga_position);
    return vga_position - begin;
}

int putfloat(double value, int flags, int width, int precision, int length)
{
    uint16_t begin = vga_position;

    putint(value, 10, flags, width, precision, length, true, false);
    put('.');

    if (value < 0.0)
        value = -value;
    value -= (int)value;
    for (int i = 0; i < precision; ++i)
        value *= 10.0;

    putint(value, 10, 0, precision, 0, 0, false, false);
    return vga_position - begin;
}
