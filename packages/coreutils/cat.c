
#include <stdint.h>

int shell_cmd_cat(int argc, char **argv)
{
	if(argc < 1)
    {
        printf("%s [path]\n", argv[0]);
        return 1;
    }

    uint32_t fd = open(argv[1], 0);

    char *buf = kmalloc(30);
    read(fd, buf, 30);

    uint32_t out = open("/tty", 0);
	write(out, buf, 30);
    printf("\n");
	return 0;
}
