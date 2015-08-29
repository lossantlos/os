#include <kernel/tty.h>
#include <kernel/vga.h>

#include <pcspkr.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

void tty_setcolor(uint8_t color)
{
	terminal_color = color;
}

#warning TODO backspace, scrooling (buffer)

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
		case '\a':
			beep();
			break;
        case '\b':
            if(terminal_column < 1) break;
            terminal_column--;
			tty_putchar(' ');
			terminal_column--;
            break;
		case '\f':
			for(unsigned int i = 0; i < VGA_HEIGHT; i++) putchar('\n');
			terminal_column = 0;
			terminal_row = 0;
			break;
		case '\r': //carrier return
			terminal_row = 0;
			break;
		case '\v': //vertical tab
			#warning TODO
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
	}
	if(terminal_row >= VGA_HEIGHT)
	{
		terminal_row = VGA_HEIGHT-1;
		vga_scrool();
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
