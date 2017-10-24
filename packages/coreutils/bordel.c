
int shell_cmd_clear(int argc, char **argv)
{
    vga_clear();
    vga_setcursor(0, 0);
    return 0;
}

int shell_cmd_zero(int argc, char **argv)
{
    printf("(1/0) = %i\n", (1/0));
    return 0;
}

/*
int shell_cmd_(int argc, char **argv)
{
    return 0;
}

int shell_cmd_(int argc, char **argv)
{
    return 0;
}

int shell_cmd_(int argc, char **argv)
{
    return 0;
}
*/
