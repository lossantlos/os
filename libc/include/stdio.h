/**
@file stdio.h
*/

#ifndef _STDIO_H
#define _STDIO_H


typedef struct {

} FILE;

extern FILE *stdin;  ///<Standart input stream
extern FILE *stdout; ///<Standart output stream
extern FILE *stderr; ///<Standart error stream


FILE *fopen(const char *path, const char *mode);

int putc(int c, FILE *stream);
int puts(const char *s);
int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int putchar(int c);

int printf(const char *format, ...); //print string specified by format

#endif
