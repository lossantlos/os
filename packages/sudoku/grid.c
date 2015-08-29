//
//  grid.c
//  sudoku
//
//  Created by Jakub Kyzek on 17.08.15.
//  Copyright (c) 2015 kyzek. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"

#warning TODO error checking
int grid_import(const char *name, grid_t g)
{
	FILE *file = fopen(name, "r");
	
	if(!file)
		return -1; //ERROR
	
	int x = 0, y = 0;
	char c = 0;
	while(!feof(file))
	{
		fscanf(file, "%c", &c);
		grid_set(g, x, y, (c - '0'));
		if(++y > 8)
		{
			if(++x == 9) return 0;
			y = 0;
		}
	}
	
	return 0;
}

cell_t grid_get_cell(grid_t g, int x, int y)
{
	cell_t a;
	a.raw = ((g[x][y/3] >> (2-(y % 3)) * 5) & 0b11111);
	return a;
}

inline void grid_set_raw(grid_t g, int x, int y, uint16_t c)
{
	g[x][y/3] |= ((c & 0b11111) << (2-(y % 3)) * 5);
}

void grid_set(grid_t g, int x, int y, uint16_t c)
{
	if(!grid_cell_ro(g, x, y)) grid_set_raw(g, x, y, c);
}


void grid_print(grid_t g)
{
	const char *s = "+-------+-------+-------+\n";
	printf("%s", s);
	for(int x = 0; x < 9; x++)
	{
		printf("|");
		for(int y = 0; y < 9; y++)
		{
			if(!grid_get(g, x, y)) printf("  ");
			else printf(" %i", grid_get(g, x, y));
			if(!((y+1) % 3)) printf(" |");
		}
		printf("\n");
		if(!((x+1) % 3)) printf("%s", s);
	}
}

uint16_t **grid_calloc()
{
	grid_t g = (grid_t) calloc(10, sizeof(grid_t));
	
	for(int x = 0; x < 10; x++)
		g[x] = (uint16_t *) calloc(3, sizeof(uint16_t *));
	
	return g;
}

void grid_free(grid_t g)
{
	for(int x = 0; x < 10; x++)
		free(g[x]);
	free(g);
}

int grid_check(grid_t g)
{
	//int *c = malloc(10 * sizeof(int));
	int c[10];
	memset(c, 0, 10 * sizeof(int));
	
	//row
	for(int x = 0; x < 10; x++)
	{
		for(int y = 0; y < 10; y++)
			c[grid_get(g, x, y)]++;
		
		for(int q = 1; q < 10; q++)
			if(c[q] > 1)
				return 1;
		
		memset(c, 0, 10 * sizeof(int));
	}
	
	//column
	for(int y = 0; y < 10; y++)
	{
		for(int x = 0; x < 10; x++)
			c[grid_get(g, x, y)]++;
		
		for(int q = 1; q < 10; q++)
			if(c[q] > 1)
				return 1;
		
		memset(c, 0, 10 * sizeof(int));
	}
	
	//cell
	for(int r = 0; r < 9; r += 3)
	{
		for(int s = 0; s < 9; s += 3)
		{
			for(int x = 0; x < 3; x += 1)
				for(int y = 0; y < 3; y += 1)
					c[grid_get(g, r+x, s+y)]++;
			
			for(int q = 1; q < 10; q++)
				if(c[q] > 1)
					return 1;
			
			memset(c, 0, 10 * sizeof(int));
		}
	}
	
	return 0;
}

void grid_generate(grid_t g, gen_difficulty dif)
{
#warning TODO
	grid_import("./test_grid.txt", g);
}

