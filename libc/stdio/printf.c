#include "../include/stdio.h"

#include <stdarg.h>

int printf(const char *format, ...)
{
    va_list parameters;
	va_start(parameters, format);

    while(*format != '\0')
    {
        if (*format == '%')
        {
            format++;
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
        else {
            putchar(*format);
            format++;
        }

    }

    va_end(parameters);
}
