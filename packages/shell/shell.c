
#include <stdio.h>
#include <stdlib.h>

//------------------

#include <stdlib.h>

char *getenv(const char *name);
int putenv(char *string);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

//------

typedef struct {
	char *name;
	uint32_t size;
	char *data;
} env_var_t;

char *getenv(const char *name)
{

}

int setenv(const char *name, const char *value, int overwrite)
{

}

int unsetenv(const char *name)
{

}

int shell_cmd_help(int argc, char **argv)
{
	printf("\
There are few commands:\n\
\techo\t - print all arguments\n\
\thelp\t - print this help\n\
\tinvaders - run invaders game\n\
\tauthor\t - print author\n\
\thalt\t - halt computer\n");
	return 0;
}

int shell_cmd_author(int argc, char **argv)
{
	printf("Jakub Kyzek (2016)\n");
	return 0;
}


int shell_cmd_halt(int argc, char **argv)
{
	#warning TODO implement acpi halt

	printf("System halted!\n");
	__asm__ ("cli");
	__asm__ ("hlt");
	return 0;
}

extern int shell_cmd_invaders(int argc, char **argv);
extern int shell_cmd_echo(int argc, char **argv);
extern int shell_cmd_cpuid(int argc, char **argv);
extern int shell_cmd_memdump(int argc, char **argv);
extern int shell_cmd_cat(int argc, char **argv);
extern int shell_cmd_date(int argc, char **argv);
extern int shell_cmd_lspci(int argc, char **argv);
extern int shell_cmd_ls(int argc, char **argv);
extern int shell_cmd_clear(int argc, char **argv);
extern int shell_cmd_zero(int argc, char **argv);

#define CMD(NAME) { #NAME, &shell_cmd_ ## NAME }
struct
{
	char *name;
	int (*fce) (int argc, char **argv);
} shell_cmd[] = {
	CMD(echo),
	CMD(help),
	CMD(invaders),
//	CMD(sudoku),
	CMD(cat),
	CMD(date),
	CMD(author),
	CMD(halt),
	CMD(cpuid),
	CMD(memdump),
	CMD(lspci),
	CMD(ls),
	CMD(clear),
	CMD(zero),
	{NULL, NULL}
};
#undef CMD


#warning TODO rewrite with dynamic memory

void shell()
{
	int argc;
	char *argv[10];
	char buffer[512];
	int pos = 0;

	while (1)
	{
		printf("> ");
		argc = 0, pos = 0;
		memset(buffer, 0, 512);
		memset(argv, NULL, 10);
		argv[0] = &buffer[0];

		char c = '\0';
		while(c != '\n')
		{
			c = (char)getchar();

			if(c == '\b')
			{
				if(pos < 1) continue;
				putchar(c);
				buffer[--pos] = '\0';
			} else if(c == '\t')
			{
				int count = 0, addr = 0; //auto-indent

				for(int x = 0; shell_cmd[x].name; x++)
					if(!memcmp(argv[0], shell_cmd[x].name, pos))
					{
						count++;
						addr = x;
					}

				if(count == 0);
				else if(count == 1)
				{
					for(; shell_cmd[addr].name[pos]; pos++)
						putchar((buffer[pos] = shell_cmd[addr].name[pos]));
					putchar((buffer[pos++] = ' '));
				} else
				{
					printf("\n");
					for(int x = 0; shell_cmd[x].name; x++)
						if(!memcmp(argv[0], shell_cmd[x].name, pos))
							printf("%s, ", shell_cmd[x].name);
					printf("\b\b \n> %s", buffer);
				}

			} else {
				putchar((buffer[pos++] = c));
			}
		}

		if(pos<2) continue;

		for (int x = 0; x < pos; x++)
		{
			if(buffer[x] == ' ')// && buffer[x+1] != '\n')
			{
				buffer[x] = '\0';
				argv[++argc] = &buffer[++x];
			}
		}

		buffer[pos-1] = '\0';

		int cmd_found = 0;
		for (int i = 0; shell_cmd[i].name != NULL; i++) {
			if(!strcmp(argv[0], shell_cmd[i].name))
			{
//				printf("\n + history: %s\n", buffer);
				int r = shell_cmd[i].fce(argc, argv);
				cmd_found = 1;
				break;
			}
		}
		if(!cmd_found) printf("Command not found!\n");
	}
}
