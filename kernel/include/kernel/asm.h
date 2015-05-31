

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}

static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    __asm__ volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}
