//
//  game.c
//  sudoku
//
//  Created by Jakub Kyzek on 17.08.15.
//  Copyright (c) 2015 kyzek. All rights reserved.
//

#include "game.h"
#include "grid.h"

#include <stdlib.h>

//time
grid_t g = NULL;

#define pause() {getc(stdin); getc(stdin);}

void game_help()
{
#warning TODO write help
	printf("Help!!!!!!!!!!!");
	pause();
	game_menu();
}

void game_about()
{
	printf("Author is Jakub Kyzek (2015).\n");
	pause();
	game_menu();
}

void game_exit()
{
	if(g) grid_free(g);
	exit(0);
}

void game()
{
#warning TODO
	//grid[0][1] = 9;
	while(1) {
		grid_print(g);
		printf("<row>x<column> <number>: ");
		int x, y, i;
		scanf("%ix%i %i", &x, &y, &i);
		printf("%ix%i %i\n", x, y, i);
	}
}

void game_reset()
{
	if(g) grid_free(g);
	g = grid_calloc();
}

void game_import()
{
	printf("Enter file name: ");
	char buf[64];
	fgets(buf, sizeof(buf), stdin);
	game_reset();
	grid_import(buf, g);
	game();
}

void game_easy()
{
	game_reset();
	grid_generate(g, easy);
	game();
}

void game_medium()
{
	game_reset();
	grid_generate(g, medium);
	game();
}

void game_hard()
{
	game_reset();
	grid_generate(g, hard);
	game();
}

typedef struct _menu_t
{
	const char *entry;
	void (*fce)(struct _menu_t *);
	struct _menu_t *arg;
} menu_t;

void menu(menu_t *m);

menu_t menu_new_game[],
menu_generate[],
menu_load_file[],
menu_main[] = {
	"New game",			menu,			menu_new_game,
	"About",			game_about,		NULL,
	"Help",				game_help,		NULL,
	"Exit",				game_exit,		NULL,
	NULL,				NULL,			NULL
}, menu_new_game[] = {
	"Generate",			menu,			menu_generate,
	"Load from file",	menu,			menu_load_file,
	"Back",				menu,			menu_main,
	NULL,				NULL,			NULL
}, menu_generate[] = {
	"Easy",				game_easy,		NULL,
	"Medium",			game_medium,	NULL,
	"Hard",				game_hard,		NULL,
	"Back",				menu,			menu_new_game,
	NULL,				NULL,			NULL,
}, menu_load_file[] = {
	"Select file",		game_import,	NULL,
	"Back",				menu,			menu_new_game,
	NULL,				NULL,			NULL,
};



void menu(menu_t *m)
{
	int x;
	for (x = 0; m[x].entry; x++) {
		printf("%s%i | %s\n", "\t", x+1, m[x].entry);
	}
	
	int c = 0;
	do {
		scanf("%i", &c);
	} while(!c || --c > x);
	
	menu_t *a = &m[c];
	
#warning TODO recursive function call?
	if (a->fce) a->fce(a->arg);
	
}


void game_menu()
{
	menu(menu_main);
}


