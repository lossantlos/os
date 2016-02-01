
void lspci_detailed()
{
    pci_print_list_detailed();
}

void lspci_simple()
{
    pci_print_list();
    printf("\n");
}


void lspci_help(char **argv)
{
    printf("\
Usage: %s [-s]\n\nArguments:\n\
  -s\tjust simple output (VENDOR:DEVICE1, VENDOR:DEVICE2)\n\
  -a\tprint all devices (verbose output)\n\n", argv[0]);
    return 1; //should be exit but not yet implemented
}

int shell_cmd_lspci(int argc, char **argv)
{
    #warning TODO trouble with argc
    if(argc == 1)
    {
        if(!strcmp(argv[1], "-s")) lspci_simple();
        else if(!strcmp(argv[1], "-a")) lspci_detailed();
        else lspci_help(argv);
    } else lspci_help(argv);

	return 0;
}
