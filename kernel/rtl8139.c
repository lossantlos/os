//--------------- RTL8139 -----------------

#include <kernel/pci.h>
#include <kernel/irq.h>
#include <kernel/asm.h>

uint32_t ioaddr;

void rtl8139_handler(struct regs *r)
{
    outw(ioaddr + 0x3E, 0x1); // Interrupt Status - Clears the Rx OK bit, acknowledging a packet has been received,
    printf(".\n\n\n");
}

void rtl8139_init(uint16_t bus, uint16_t device, uint8_t funct)
{
    pci_device_info dev;
    pci_device_info_read(bus, device, funct, &dev);
    if(dev.vendor != 0x10ec || dev.device != 0x8139) return;
    printf("Initializing 8139:\n");
    ioaddr = (uint32_t)(dev.pdh0h.BAR0 & 0xfffffffc);

    pci_config_write16(bus, device, funct, 0x04, (dev.command | 0b100));
    pci_device_info_read(bus, device, funct, &dev);
    printf("ioaddr = 0x%x\ninterrupt_pin: %i\ninterrupt_line: %i\n", ioaddr, dev.pdh0h.interrupt_pin, dev.pdh0h.interrupt_line);

    //Turning on
    outb(ioaddr + 0x52, 0x0);

    //Software reset
    outb(ioaddr + 0x37, 0x10);
    while((inb(ioaddr + 0x37) & 0x10) != 0);

    //Print mac address
    outb(ioaddr+1, 0x12);
    for(uint8_t i = 0; i < 6; i++)
        printf("%02x:", inb(ioaddr + i*sizeof(uint8_t)));
    printf("\b \n");

    //Init receive buffer
    uint32_t *rx_buffer_virtual = malloc(8192 + 16); //(8K + 16 bytes)
    uint32_t *rx_buffer = rx_buffer_virtual - 0xC0000000;

    outl(ioaddr + 0x30, (uint32_t *)rx_buffer); // send uint32_t memory location to RBSTART (0x30)

    //Set IMR + ISR
    outw(ioaddr + 0x3C, 0x0005); // Sets the TOK and ROK bits high

    //Configuring receive buffer
    outl(ioaddr + 0x44, 0xf | (1 << 7)); // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP

    //Enable Receive and Transmitter
    outb(ioaddr + 0x37, 0x0C); // Sets the RE and TE bits high

    irq_handler_set(IRQ11, rtl8139_handler);

    //ISR Handler
    outw(ioaddr + 0x3E, 0x1); // Interrupt Status - Clears the Rx OK bit, acknowledging a packet has been received,
                               // and is now in rx_buffer

//    printf("\n");
}
