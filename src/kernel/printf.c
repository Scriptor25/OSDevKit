#include <scriptos/vga.h>
#include <stdarg.h>

// %[flags][width][.precision][length]specifier
int printf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // va_arg(ap, int);

    int state = PRINTF_STATE_NONE,
        flags = 0,
        width = 0,
        precision = 0,
        length = 0;

    uint16_t begin = vga_position;

    for (char *c = (char *)format; *c;)
    {
        switch (state)
        {
        case PRINTF_STATE_NONE:
            if (*c == '%')
            {
                flags = 0;
                state = PRINTF_STATE_FLAGS;
                c++;
                break;
            }
            if (*c == '\r')
            {
                vga_position -= (vga_position % VGA_WIDTH);
                c++;
                break;
            }
            if (*c == '\n')
            {
                vga_position += VGA_WIDTH - (vga_position % VGA_WIDTH);
                c++;
                break;
            }
            put(*(c++));
            break;

        case PRINTF_STATE_FLAGS:
            switch (*c)
            {
            case '-':
                flags |= PRINTF_FLAG_LEFT_JUSTIFY;
                c++;
                break;
            case '+':
                flags |= PRINTF_FLAG_FORCE_SIGN;
                c++;
                break;
            case ' ':
                flags |= PRINTF_FLAG_INSERT_BLANK;
                c++;
                break;
            case '#':
                flags |= PRINTF_FLAG_INSERT_PREFIX;
                c++;
                break;
            case '0':
                flags |= PRINTF_FLAG_LEFT_PAD;
                c++;
                break;
            default:
                width = 0;
                state = PRINTF_STATE_WIDTH;
                break;
            }
            break;

        case PRINTF_STATE_WIDTH:
            if (*c == '.')
            {
                precision = 0;
                state = PRINTF_STATE_PRECISION;
                c++;
                break;
            }
            if (*c == '*')
            {
                width = va_arg(ap, int);
                c++;
                break;
            }
            if (0x30 <= *c && *c <= 0x39)
            {
                width = 10 * width + (*c - 0x30);
                c++;
                break;
            }
            length = 0;
            state = PRINTF_STATE_LENGTH;
            break;

        case PRINTF_STATE_PRECISION:
            if (*c == '*')
            {
                precision = va_arg(ap, int);
                c++;
                break;
            }
            if (0x30 <= *c && *c <= 0x39)
            {
                precision = 10 * precision + (*c - 0x30);
                c++;
                break;
            }
            length = 0;
            state = PRINTF_STATE_LENGTH;
            break;

        case PRINTF_STATE_LENGTH:
            state = PRINTF_STATE_SPECIFIER;
            break;

        case PRINTF_STATE_SPECIFIER:
            switch (*c++)
            {
            case 'd':
            case 'i':
                putint(va_arg(ap, signed), 10, flags, width, precision, length, true, false);
                break;

            case 'u':
                putint(va_arg(ap, unsigned), 10, flags, width, precision, length, false, false);
                break;

            case 'o':
                putint(va_arg(ap, unsigned), 8, flags, width, precision, length, false, false);
                break;

            case 'x':
                putint(va_arg(ap, unsigned), 16, flags, width, precision, length, false, false);
                break;

            case 'X':
                putint(va_arg(ap, unsigned), 16, flags, width, precision, length, false, true);
                break;

            case 'f':
                putfloat(va_arg(ap, double), flags, width, precision, length);
                break;

            case 'c':
                put((char)va_arg(ap, int));
                break;

            case 's':
                for (char *str = va_arg(ap, char *); *str;)
                    put(*(str++));
                break;

            case 'p':
                putint((intptr_t)va_arg(ap, void *), 16, flags, width, precision, length, false, true);
                break;

            case 'n':
                *va_arg(ap, int *) = vga_position - begin;
                break;

            case '%':
                put('%');
                break;

            default:
                break;
            }
            state = PRINTF_STATE_NONE;
            break;

        default:
            break;
        }
    }

    va_end(ap);

    update_cursor(vga_position);
    return vga_position - begin;
}
