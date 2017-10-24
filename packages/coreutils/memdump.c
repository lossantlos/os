
#include <stdint.h>

uint32_t size = 10;

void hexdump(void *ptr)
{
    char *a = ptr;
    for(uint32_t x = 0; x < size; x++)
        printf("%02x ", (uint8_t) a[x]);
    printf("\n");
}

int shell_cmd_memdump(int argc, char **argv)
{
    printf("%i\n", argc);
    if(argc == 0)
    {
        printf("%s <address> [count]\n", argv[0]);
        return 1;
    }

    if(argc == 2) size = atoi(argv[2]);
    hexdump(atoi(argv[1]));
    return 0;
}
