#include <scriptos/io.h>

void outb(uint16_t port, uint8_t value)
{
    asm volatile(
        "out %0, %1"
        :
        : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile(
        "in %1, %0"
        : "=a"(value)
        : "Nd"(port));
    return value;
}
