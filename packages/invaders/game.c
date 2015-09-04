
#include <stdio.h>

#include <stdint.h>

#include "menu.h"

#define SCREEN_SIZE_X 80
#define SCREEN_SIZE_Y 25

#define INVADERS 59
#define INVADER_OFFSET 15
#define SHOTS_MAX 10

const char invader_graphics[] = "-*-";
const char player_graphics[] = "/-^-\\";
const char shot_graphics = '|';

struct {
    signed int x, y;
} invader[INVADERS];

struct {
    signed int x, y;
} shot[SHOTS_MAX];

struct {
    signed int x, y;
} player;

char scr[SCREEN_SIZE_Y][SCREEN_SIZE_X]; //screen

void game_reset()
{
    player.x = SCREEN_SIZE_X / 2;
    player.y = SCREEN_SIZE_Y - 1;
    uint32_t offset = 0;
    for(uint32_t x = 0; x < INVADERS; x++)
    {
        invader[x].x = offset % SCREEN_SIZE_X;
        invader[x].y = offset / SCREEN_SIZE_X;
        offset += INVADER_OFFSET;
    }
    for(uint32_t x = 0; x < SHOTS_MAX; x++)
    {
        shot[x].x = 0;
        shot[x].y = 0;
    }

}

void game_draw()
{
    printf("\n");
    for (uint32_t i = 0; i < INVADERS; i++)
        memcpy(&scr[invader[i].y][invader[i].x], &invader_graphics, strlen(invader_graphics, sizeof(char)));

    memcpy(&scr[player.y][player.x], &player_graphics, strlen(player_graphics, sizeof(char)));

    uint32_t o = open("/tty");
    write(o, &scr, sizeof(scr)-1);
}

void game()
{
    game_draw();
    game_draw();
    while (1) {}
}

int shell_cmd_invaders(int argc, char **argv)
{
    game_reset();
    game();
    //while(menu() != MENU_QUIT);
    return 0;
}
