/*
void funkce1()
{
    char a = 'e';
}

void funkce2()
{
    char a = 'e';
}
*/

#include <stdint.h>

//int main(int argc, char **argv)
void main()
{
    uint16_t *c = (uint16_t *) 0xB8000;
    c[0] = 0b01000010 << 8 | '*';

//    funkce1();
//    funkce2();
//    return 15;
}
