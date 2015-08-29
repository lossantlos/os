#include <kernel/asm.h>
#include <kernel/irq.h>

unsigned char kb_us[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

#define KB_US_LSHIFT 42
#define KB_US_RSHIFT 54

#define KB_BUFFER_SIZE 10

int kb_buffer[KB_BUFFER_SIZE];
volatile int kb_buffer_pos_write = 0, kb_buffer_pos_read = 0;

int kb_buffer_check()
{
    kb_buffer_pos_write %= KB_BUFFER_SIZE;
    kb_buffer_pos_read %= KB_BUFFER_SIZE;
    if(kb_buffer[kb_buffer_pos_write])
        printf("WARN: %s:%i: buffer overflow\n", __FILE__, __LINE__);
}

void kb_buffer_push(int key)
{
    if(!key) return;
    kb_buffer[kb_buffer_pos_write++] = key;
    kb_buffer_check();
}

int kb_buffer_pop()
{
    if(!kb_buffer[kb_buffer_pos_read]) return 0;
    int a = kb_buffer[kb_buffer_pos_read];
    kb_buffer[kb_buffer_pos_read] = 0;
    kb_buffer_pos_read++;
    kb_buffer_check();
    return a;
}


//----------------------------
#warning TODO move to another file
int getchar_nonblock()
{
    return kb_buffer_pop();
}

int getchar()
{
    while(kb_buffer_pos_write == kb_buffer_pos_read);
    return kb_buffer_pop();
}
//----------------------------
void keyboard_handler(struct regs *r)
{
    unsigned char scancode = inb(0x60);

    if (scancode & 0x80) {
        //released
    } else {
        //pressed
//        putchar(kb_us[scancode]);
        kb_buffer_push((int)kb_us[scancode]);
    }

}

void kb_init()
{
    irq_handler_set(IRQ1, keyboard_handler);
}
