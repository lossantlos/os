
#include "menu.h"

int menu()
{
    printf("MENU:\n\t1 - play\n\t2 - help\n\t3 - about\n\t4 - settings\n\t5 - quit\n");
    char choice = '\0';
    choice = getchar();
    switch (choice) {
        case '1': //play
            printf("Play\n");
            break;
        case '2': //help
            printf("This is help for Invaders game\n----------------\n\nuse keys to move spaceship...\n");
            break;
        case '3': //about
            printf("OS and game was written by Jakub Kyzek (2015)\n");
            break;
        case '4': //settings
            printf("Enter player name: \n");
            break;
        case '5': //exit
            return MENU_QUIT;
            break;
        default:
            break;
    }
    //soundtrack
    //menu
        //play
            //gameloop();
            //who win?
            //wait for keypress
            //reset game
        //help / about
        //exit
}
