#include <kernel/vga.h>
#include <kernel/asm.h>

uint16_t *vgabuf;

/**
@brief Initialize vga
*/
void vga_init()
{
    vgabuf = VGA_PTR;
    vga_clear();
    vga_setcursor(0, 0);
}

/**
@brief Clear vga
*/
void vga_clear()
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_put(' ', make_color(COLOR_LIGHT_GREY, COLOR_BLACK), x, y);
}

/**
@brief Put colored character to specific position
*/
void vga_put(char c, uint8_t color, uint8_t x, uint8_t y)
{
    vgabuf[y * VGA_WIDTH + x] = make_vgaentry(c, color);
}

void vga_scrool()
{
    memcpy(vgabuf, (vgabuf+VGA_WIDTH), VGA_WIDTH*VGA_HEIGHT*sizeof(uint16_t));
}

/**
@brief Move cursor to position
*/
void vga_setcursor(uint8_t x, uint8_t y)
{
    uint16_t p = (uint16_t) (y * VGA_WIDTH) + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(p & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(( p >> 8) & 0xFF));
}
