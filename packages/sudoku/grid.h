//
//  grid.h
//  sudoku
//
//  Created by Jakub Kyzek on 17.08.15.
//  Copyright (c) 2015 kyzek. All rights reserved.
//

#ifndef __sudoku__grid__
#define __sudoku__grid__

#include <stdint.h>


typedef uint16_t **grid_t;

typedef union
{
	uint8_t raw:5;
	struct
	{
		uint8_t val:4;
		uint8_t ro:1;
	};
} cell_t;


uint16_t	**grid_calloc();
void		grid_free(grid_t g);

int			grid_import(const char *name, grid_t g);

/*
uint8_t		grid_get(grid_t g, int x, int y);
uint8_t		grid_cell_raw(grid_t g, int x, int y);
uint8_t		grid_get_ro(grid_t g, int x, int y);
 */

cell_t		grid_get_cell(grid_t g, int x, int y);
#define		grid_get(g, x, y)		(uint8_t) grid_get_cell(g, x, y).val
#define		grid_cell_ro(g, x, y)	(uint8_t) grid_get_cell(g, x, y).ro


void		grid_set(grid_t g, int x, int y, uint16_t c);
void		grid_set_raw(grid_t g, int x, int y, uint16_t c);

void		grid_print(grid_t g);

int			grid_check(grid_t g);

typedef enum {
	easy, medium, hard
} gen_difficulty;

void		grid_generate(grid_t g, gen_difficulty dif);



#endif /* defined(__sudoku__grid__) */
