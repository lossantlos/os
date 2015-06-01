#include <kernel/tty.h>
#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

void tty_setcolor(uint8_t color)
{
	terminal_color = color;
}

void tty_putchar(char c)
{
    switch (c) {
        case '\n':
            terminal_column = 0;
            terminal_row++;
            break;
        case '\t':
            terminal_column += 4 - terminal_column % 4;// %= 4;
            break;
        case '\b':
            #warning TODO backspace chracter
            terminal_row--;
            break;
        default:
            vga_put(c, terminal_color, terminal_column, terminal_row);
            terminal_column++;
            break;
    }

	if(terminal_column >= VGA_WIDTH)
	{
		terminal_column = 0;
		terminal_row++;
		if(terminal_row >= VGA_HEIGHT) terminal_row = 0;
	}

    vga_setcursor(terminal_column, terminal_row);
}

void tty_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for(size_t i = 0; i < datalen; i++) tty_putchar(data[i]);
}

void tty_init()
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}
