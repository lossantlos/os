
#define panic(msg) {printf("KERNEL PANIC!!! %s:%i: %s\n", __FILE__, __LINE__, msg); cli(); for(;;);};
