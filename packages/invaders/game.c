/**
@file game.c
*/

#include <kernel/vga.h>
#include <kernel/asm.h>

#include <stdio.h>
#include <stdint.h>

#include "menu.h"

#define SCREEN_SIZE_X   80
#define SCREEN_SIZE_Y   25

#define INVADERS        40
#define INVADER_OFFSET  22
#define SHOTS_MAX       3

const char invader_graphics[] = "-*-";
const char player_graphics[] = "/-^-\\";
const char shot_graphics[] = "|";

typedef struct {
    signed int  x, y;
    uint8_t     graphics_len;
    const char  *graphics;
} object_t;

#define OBJECT_NOEXIST -1 ///<Value in x coordinate when object doesnt exist

object_t invader[INVADERS];
object_t shot[SHOTS_MAX];
object_t player;

char scr[SCREEN_SIZE_Y][SCREEN_SIZE_X]; //screen

void game_reset()
{
    player.graphics = player_graphics;
    player.graphics_len = strlen(player_graphics);
    player.x = SCREEN_SIZE_X / 2;
    player.y = SCREEN_SIZE_Y - 1;
    uint32_t offset = 0;
    for(uint32_t x = 0; x < INVADERS; x++)
    {
        invader[x].graphics = invader_graphics;
        invader[x].graphics_len = strlen(invader_graphics);
        invader[x].x = offset % SCREEN_SIZE_X;
        invader[x].y = offset / SCREEN_SIZE_X;
        offset += INVADER_OFFSET;
    }
    for(uint32_t x = 0; x < SHOTS_MAX; x++)
    {
        shot[x].graphics = shot_graphics;
        shot[x].graphics_len = strlen(shot_graphics);
        shot[x].x = -1;
        shot[x].y = -1;
    }

}

void vga_cursor_hide()
{
    outb(0x3D4, 0x0a);
    outb(0x3D5, (uint8_t)(0b00100000));
}

void vga_cursor_show()
{
    outb(0x3D4, 0x0a);
    outb(0x3D5, (uint8_t)(0b00000000));
}

void game_draw()
{
//    printf("\n");
    memset(scr, ' ', SCREEN_SIZE_X*SCREEN_SIZE_Y);

    for(uint8_t x = 0; x < SHOTS_MAX; x++)
        if(shot[x].x >= 0) memcpy(&scr[shot[x].y][shot[x].x], shot[x].graphics, shot[x].graphics_len * sizeof(char));

    for (uint32_t i = 0; i < INVADERS; i++)
        if(invader[i].x >= 0) memcpy(&scr[invader[i].y][invader[i].x], invader[i].graphics, invader[i].graphics_len * sizeof(char));

    memcpy(&scr[player.y][player.x], player.graphics, player.graphics_len * sizeof(char));

    for(uint32_t x = 0; x < SCREEN_SIZE_X; x++)
        for(uint32_t y = 0; y < SCREEN_SIZE_Y; y++)
            if(scr[y][x]) vga_put(scr[y][x], make_color(COLOR_WHITE, COLOR_BLACK), (uint8_t)x, (uint8_t)y);
}

void win()
{
    printf("Player WIN!!!");
    pic_wait(10);
}

void lose()
{
    printf("Haha!!!");
    pic_wait(10);
}

void game()
{
    vga_cursor_hide();
    game_draw();

    uint8_t k = 0;

    while (1) {
        pic_wait(1);
        //input
        while((k = getchar_nonblock()))
        {
            if(k == 'a' && player.x > 0) player.x--;
            else if(k == 'd' && player.x < SCREEN_SIZE_X - player.graphics_len) player.x++;
            else if(k == ' ')
            {
                for(uint16_t x = 0; x <= SHOTS_MAX; x++)
                {
                    if(shot[x].x == -1)
                    {
                        shot[x].x = player.x + (player.graphics_len / 2);
                        shot[x].y = player.y - 1;
                        break;
                    }


                }
            }
            else if(k == 'q')
            {
                vga_cursor_show();
                printf("\f\n\f");
                return;
            }
        }
        //move shots forward
        for(uint8_t x = 0; x < SHOTS_MAX; x++)
            if(shot[x].x >= 0 && shot[x].y >= 0) if(shot[x].y-- < 1) shot[x].x = -1;

        //move invaders forward
        for(uint32_t x = 0; x < INVADERS; x++)
        {
            if(invader[x].x >= 0)
            if((invader[x].x += 1) > SCREEN_SIZE_X)
            {
                invader[x].x %= SCREEN_SIZE_X;
                if(invader[x].y++ >= SCREEN_SIZE_Y) return lose(); //TODO player looses
            }
        }

        //collisions
        for(uint32_t m = 0; m < INVADERS ; m++)
            for(uint32_t n = 0; n < SHOTS_MAX; n++)
            {
                if( (invader[m].x >= 0 && shot[n].x >= 0) && //does it exist?
                    (invader[m].x <= shot[n].x && invader[m].x + invader[m].graphics_len >= shot[n].x) &&
                    invader[m].y == shot[n].y)
                {
                    invader[m].x = -1;
                    shot[n].x = -1;
                }
            }

        game_draw();

    }
}

int shell_cmd_invaders(int argc, char **argv)
{
    game_reset();
    game();
    //while(menu() != MENU_QUIT);
    return 0;
}
