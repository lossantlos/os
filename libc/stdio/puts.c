#include "../include/stdio.h"

int puts(const char *s)
{
  for (int x = 0; s[x]; x++) putchar(s[x]);
}
