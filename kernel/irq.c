#include <kernel/irq.h>
#include <kernel/asm.h>

extern void irq0(), irq1(), irq2(), irq3(), irq4(), irq5(), irq6(), irq7(),
irq8(), irq9(), irq10(), irq11(), irq12(), irq13(), irq14(), irq15(), irq16(),
irq17(), irq18(), irq19(), irq20(), irq21(), irq22(), irq23(), irq24(), irq25(),
irq26(), irq27(), irq28(), irq29(), irq30(), irq31(), irq32(), irq33(), irq34(),
irq35(), irq36(), irq37(), irq38(), irq39(), irq40(), irq41(), irq42(), irq43(),
irq44(), irq45(), irq46(), irq47(), irq48(), irq49(), irq50(), irq51(), irq52(),
irq53(), irq54(), irq55(), irq56(), irq57(), irq58(), irq59(), irq60(), irq61(),
irq62(), irq63(), irq64(), irq65(), irq66(), irq67(), irq68(), irq69(), irq70(),
irq71(), irq72(), irq73(), irq74(), irq75(), irq76(), irq77(), irq78(), irq79(),
irq80(), irq81(), irq82(), irq83(), irq84(), irq85(), irq86(), irq87(), irq88(),
irq89(), irq90(), irq91(), irq92(), irq93(), irq94(), irq95(), irq96(), irq97(),
irq98(), irq99(), irq100(), irq101(), irq102(), irq103(), irq104(), irq105(),
irq106(), irq107(), irq108(), irq109(), irq110(), irq111(), irq112(), irq113(),
irq114(), irq115(), irq116(), irq117(), irq118(), irq119(), irq120(), irq121(),
irq122(), irq123(), irq124(), irq125(), irq126(), irq127(), irq128(), irq129(),
irq130(), irq131(), irq132(), irq133(), irq134(), irq135(), irq136(), irq137(),
irq138(), irq139(), irq140(), irq141(), irq142(), irq143(), irq144(), irq145(),
irq146(), irq147(), irq148(), irq149(), irq150(), irq151(), irq152(), irq153(),
irq154(), irq155(), irq156(), irq157(), irq158(), irq159(), irq160(), irq161(),
irq162(), irq163(), irq164(), irq165(), irq166(), irq167(), irq168(), irq169(),
irq170(), irq171(), irq172(), irq173(), irq174(), irq175(), irq176(), irq177(),
irq178(), irq179(), irq180(), irq181(), irq182(), irq183(), irq184(), irq185(),
irq186(), irq187(), irq188(), irq189(), irq190(), irq191(), irq192(), irq193(),
irq194(), irq195(), irq196(), irq197(), irq198(), irq199(), irq200(), irq201(),
irq202(), irq203(), irq204(), irq205(), irq206(), irq207(), irq208(), irq209(),
irq210(), irq211(), irq212(), irq213(), irq214(), irq215(), irq216(), irq217(),
irq218(), irq219(), irq220(), irq221(), irq222(), irq223(), irq224();


/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[256] = {0};

/* This installs a custom IRQ handler for the given IRQ */
void irq_handler_set(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_handler_free(int irq)
{
    irq_routines[irq] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_init()
{
    //remap IRQ
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

//    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);

    #define ISG(x) idt_set_gate(32 + x, (unsigned)irq ## x, 0x08, 0x8E)

    ISG(0); ISG(1); ISG(2); ISG(3); ISG(4); ISG(5); ISG(6); ISG(7); ISG(8);
    ISG(9); ISG(10); ISG(11); ISG(12); ISG(13); ISG(14); ISG(15); ISG(16);
    ISG(17); ISG(18); ISG(19); ISG(20); ISG(21); ISG(22); ISG(23); ISG(24);
    ISG(25); ISG(26); ISG(27); ISG(28); ISG(29); ISG(30); ISG(31); ISG(32);
    ISG(33); ISG(34); ISG(35); ISG(36); ISG(37); ISG(38); ISG(39); ISG(40);
    ISG(41); ISG(42); ISG(43); ISG(44); ISG(45); ISG(46); ISG(47); ISG(48);
    ISG(49); ISG(50); ISG(51); ISG(52); ISG(53); ISG(54); ISG(55); ISG(56);
    ISG(57); ISG(58); ISG(59); ISG(60); ISG(61); ISG(62); ISG(63); ISG(64);
    ISG(65); ISG(66); ISG(67); ISG(68); ISG(69); ISG(70); ISG(71); ISG(72);
    ISG(73); ISG(74); ISG(75); ISG(76); ISG(77); ISG(78); ISG(79); ISG(80);
    ISG(81); ISG(82); ISG(83); ISG(84); ISG(85); ISG(86); ISG(87); ISG(88);
    ISG(89); ISG(90); ISG(91); ISG(92); ISG(93); ISG(94); ISG(95); ISG(96);
    ISG(97); ISG(98); ISG(99); ISG(100); ISG(101); ISG(102); ISG(103); ISG(104);
    ISG(105); ISG(106); ISG(107); ISG(108); ISG(109); ISG(110); ISG(111);
    ISG(112); ISG(113); ISG(114); ISG(115); ISG(116); ISG(117); ISG(118);
    ISG(119); ISG(120); ISG(121); ISG(122); ISG(123); ISG(124); ISG(125);
    ISG(126); ISG(127); ISG(128); ISG(129); ISG(130); ISG(131); ISG(132);
    ISG(133); ISG(134); ISG(135); ISG(136); ISG(137); ISG(138); ISG(139);
    ISG(140); ISG(141); ISG(142); ISG(143); ISG(144); ISG(145); ISG(146);
    ISG(147); ISG(148); ISG(149); ISG(150); ISG(151); ISG(152); ISG(153);
    ISG(154); ISG(155); ISG(156); ISG(157); ISG(158); ISG(159); ISG(160);
    ISG(161); ISG(162); ISG(163); ISG(164); ISG(165); ISG(166); ISG(167);
    ISG(168); ISG(169); ISG(170); ISG(171); ISG(172); ISG(173); ISG(174);
    ISG(175); ISG(176); ISG(177); ISG(178); ISG(179); ISG(180); ISG(181);
    ISG(182); ISG(183); ISG(184); ISG(185); ISG(186); ISG(187); ISG(188);
    ISG(189); ISG(190); ISG(191); ISG(192); ISG(193); ISG(194); ISG(195);
    ISG(196); ISG(197); ISG(198); ISG(199); ISG(200); ISG(201); ISG(202);
    ISG(203); ISG(204); ISG(205); ISG(206); ISG(207); ISG(208); ISG(209);
    ISG(210); ISG(211); ISG(212); ISG(213); ISG(214); ISG(215); ISG(216);
    ISG(217); ISG(218); ISG(219); ISG(220); ISG(221); ISG(222); ISG(223);
    ISG(224);

}

unsigned char *exception_messages[] = //exception_message[interrupt_number]
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor Exception",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check (486+)",
    "Machine Check (Pentium/586+)",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void fault_handler(struct regs *r)
{
    irq_handler(r);
}


/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void irq_handler(struct regs *r)
{
    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40) outb(0xA0, 0x20);

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);

    void (*handler)(struct regs *r) = NULL;

//    handler = irq_routines[r->int_no - 32];
    handler = irq_routines[r->int_no];

    if (handler) handler(r);
    else if (r->int_no < 32) //fault? (0 to 31)
    {
        printf("%s Exception. System Halted!\n", exception_messages[r->int_no]);
        for(;;);
    }
    else
    {
        printf("Interrupt %i!\n", r->int_no);
    }

}
