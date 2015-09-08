#include "../include/string.h"

size_t strlen(const char *s)
{
  size_t x;
  for (x = 0; s[x]; x++);
  return x;
}
