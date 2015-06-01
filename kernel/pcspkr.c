#include <stdint.h>
#include <stddef.h>
#include <kernel/asm.h>

static void play_sound(uint32_t nFrequence)
{
    //set the PIT to the desired frequency
    uint32_t div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (div) );
	outb(0x42, (uint8_t) (div >> 8));

    //play the sound
    uint8_t tmp = inb(0x61);
	if(tmp != (tmp | 3)) outb(0x61, tmp | 3);
}

static void nosound()
{
	outb(0x61, (uint8_t) (inb(0x61) & 0xFC));
}

void beep()
{
    play_sound(1000);
	timer_wait(7);
    nosound();
 }
