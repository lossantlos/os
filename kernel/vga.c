#include <kernel/vga.h>

uint16_t *vgabuf;

void vga_init()
{
    vgabuf = (uint16_t *) VGA_PTR;
    vga_clear();
    vga_setcursor(0, 0);
}

void vga_clear()
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_put(' ', make_color(COLOR_LIGHT_GREY, COLOR_BLACK), x, y);
}

void vga_put(char c, uint8_t color, uint8_t x, uint8_t y)
{
    vgabuf[y * VGA_WIDTH + x] = make_vgaentry(c, color);
}

void vga_setcursor(uint8_t x, uint8_t y)
{
    uint16_t p = (uint16_t) (y * VGA_WIDTH) + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(p & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(( p >> 8) & 0xFF));
}