
#include <stdint.h>

extern char _binary_initrd_tar_start[];

int shell_cmd_ls(int argc, char **argv)
{
    tar_list_all(_binary_initrd_tar_start);
	return 0;
}
