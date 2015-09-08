
#ifndef _SERIAL_
#define _SERIAL_

/**
@file serial.h
@brief Comunication thru COM1
*/

#define PORT 0x3f8 ///<COM1

/**
@brief Initialize COM1
*/
void serial_init();

/**
@brief Read from COM1
@return character readed from COM1
*/
char serial_read();

/**
@brief Write character to COM1
@return Received character
*/
void serial_write(char a);

#endif
