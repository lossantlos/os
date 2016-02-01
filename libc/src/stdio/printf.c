#include "../include/stdio.h"

#include <stdarg.h>
#include <stdint.h>

//%[flags][width][.precision][length]specifier

/*
flags:
-	Left-justify within the given field width; Right justification is the default
+	Forces to preceed the result with a plus or minus sign (+ or -) even for positive numbers.
(space)	If no sign is going to be written, a blank space is inserted before the value.
#	Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
Used with a, A, e, E, f, F, g or G it forces the written output to contain a decimal point even if no more digits follow. By default, if no digits follow, no decimal point is written.
0	Left-pads the number with zeroes (0) instead of spaces when padding is specified.

width:
(number)	Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
*	The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.

precision:
.number	For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
For a, A, e, E, f and F specifiers: this is the number of digits to be printed after the decimal point (by default, this is 6).
For g and G specifiers: This is the maximum number of significant digits to be printed.
For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.
If the period is specified without an explicit value for precision, 0 is assumed.
.*	The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.


*/

int isdigit(int c) // ctype.h
{
    if(c <= '9' && c >= '0') return 1;
    return 0;
}

uint8_t flags = 0;
#define PFF_JUSTIFY_LEFT    (1 << 0)
#define PFF_SIGN            (1 << 1) //force print sign
#define PFF_NOSIGN          (1 << 2) //blank space instead sign
#define PFF_TYPE            (1 << 3) //print 0x or 0b before number
#define PFF_ZERO            (1 << 4) //use zero instead space

int16_t precision = -1;
int16_t width = -1;

char *getint(uint32_t n, uint8_t base)
{
    if(n == 0) return "0";
    static char buf[11] = {0};
    uint32_t val = n;
	uint32_t i = sizeof(buf);
	for(; val && i ; --i, val /= base) buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}

int putintf(uint32_t n, uint8_t base)
{
    char filling = (flags & PFF_ZERO) ? '0' : ' ';
    char *a = getint(n, base);

    if(flags & PFF_SIGN)
    {
        //print sign
        width -= (width == -1) ? 0 : 1;
    }

    if(flags & PFF_TYPE)
    {
        if(base == 2)
        {
            puts("0b");
            flags |= PFF_ZERO;
            width -= (width == -1) ? 0 : 2;
        } else if(base == 16)
        {
            puts("0x");
            flags |= PFF_ZERO;
            width -= (width == -1) ? 0 : 2;
        }
    }

#warning TODO nosign and precision

    if((strlen(a) > width) && width != -1) puts("OVF");
    else if(strlen(a) < width)
    {
        if(flags & PFF_JUSTIFY_LEFT)
        {
            puts(a);
            for(uint32_t x = 0; x < (width - strlen(a)); x++) putchar(' ');
        } else
        {
            for(uint32_t x = 0; x < (width - strlen(a)); x++) putchar((flags & PFF_ZERO) ? '0' : ' ');
            puts(a);
        }
    }
    else puts(a);
}

#define putint putintf

int printf(const char *format, ...)
{
    va_list parameters;
	va_start(parameters, format);

    while(*format)
    {
        if (*format == '%')
        {
            flags = 0;
            precision = -1;
            width = -1;

            format++;
            switch (*format) {
                case '+':
                    flags |= PFF_SIGN;
                    format++;
                    break;
                case '-':
                    flags |= PFF_JUSTIFY_LEFT;
                    format++;
                    break;
                case '#':
                    flags |= PFF_TYPE;
                    format++;
                    break;
                case ' ':
                    flags |= PFF_NOSIGN;
                    format++;
                    break;
                case '0':
                    flags |= PFF_ZERO;
                    format++;
                    break;
                default:
                    break;
            }

            if(*format == '*')
            {
                width = va_arg(parameters, int);
                format++;
            } else
            {
                for(; isdigit(*format); format++)
                    width = (width == -1) ? *format - '0' : (width * 10) + *format - '0';
            }

            if(*format == '.')
            {
                format++;
                if(*format == '*') precision = va_arg(parameters, int);
                else
                {
                    for(; isdigit(*format); format++)
                        precision = (precision == -1) ? *format - '0' : (precision * 10) + *format - '0';
                }
            }

            switch (*format) {
                case 'c':
                    putchar((char) va_arg(parameters, int));
                    break;
                case 'b':
                    putint((int) va_arg(parameters, int), 2);
                    break;
                case 'o':
                    putint((int) va_arg(parameters, int), 8);
                    break;
                case 'i':
                    putint((int) va_arg(parameters, int), 10);
                    break;
                case 'x':
                    putint((int) va_arg(parameters, int), 16);
                    break;
                case 'd':
                #warning TODO
                    putint((int) va_arg(parameters, int), 10);
                    break;
                case 'p':
                    putint((int) va_arg(parameters, int), 16);
                    break;
                case 's':
                    puts((char *) va_arg(parameters, const char *));
                    break;
                default:
                    break;
            }
            format++;
        }
        else
        {
            putchar(*(format++));
        }
    }
    va_end(parameters);
}

#undef putintf
