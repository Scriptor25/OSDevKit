#include <scriptos/io.h>
#include <scriptos/vga.h>

void main(void)
{
    set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLUE);
    enable_cursor(14, 15);
    update_cursor(0);
    clear_screen();

    for (unsigned i = 0;; i += 0x190)
    {
        update_cursor(0);
        mem_dump((void *)i, (void *)(i + 0x190));

        for (unsigned n = 0; n < 0xffffffff; ++n)
            ;
    }
}
