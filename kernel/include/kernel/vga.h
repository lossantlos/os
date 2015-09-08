
#ifndef _VGA_H
#define _VGA_H

/**
@file vga.h
@brief Printing on screen thru vga
*/

#include <stdint.h>
#include <stddef.h>

enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

#define VGA_WIDTH	80
#define VGA_HEIGHT	25
#define VGA_PTR		(uint16_t *) 0xb8000 ///< Pointer to vga memory

inline uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

inline uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void vga_init();
void vga_clear();
void vga_setcursor(uint8_t x, uint8_t y);
void vga_put(char c, uint8_t color, uint8_t x, uint8_t y);

#endif
