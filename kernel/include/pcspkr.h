#ifndef _PCSPKR_H
#define _PCSPKR_H

#include <stdint.h>

/**
@file pcspkr.h
*/

void play_sound(uint32_t nFrequence);
void nosound();

/**
@brief Use speaker for beep
*/
void beep();

#endif
