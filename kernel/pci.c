#include <kernel/pci.h>

#include <kernel/asm.h>



#define pciConfigReadWord pci_config_read16



void pci_config_write32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data) {
    uint32_t address = (uint32_t)(
        ((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)func << 8) |
        (offset & 0xfc) |
        ((uint32_t)0x80000000));

    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, data);
}

void pci_config_write16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data) {
    uint32_t address = (uint32_t)(
        ((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)func << 8) |
        (offset & 0xfc) |
        ((uint32_t)0x80000000));

    outl(PCI_CONFIG_ADDRESS, address);

//    outw(((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff), data);

//    outw(((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff), data);
    outw(PCI_CONFIG_DATA + (offset & 0x02) * 8, data);
}

uint16_t pci_config_read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    //create configuration address
    uint32_t address = (uint32_t)(
        ((uint32_t)bus << 16) |
        ((uint32_t)slot << 11) |
        ((uint32_t)func << 8) |
        (offset & 0xfc) |
        ((uint32_t)0x80000000));

    //write out the address
    outl(0xCF8, address);

    //read in the data
    //(offset & 2) * 8) = 0 will choose the first word of the 32 bits register
    return (uint16_t) ((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
}

uint32_t pci_config_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    //create configuration address
    uint32_t address = (uint32_t)
        (uint32_t)0x80000000 |
        (uint32_t)bus << 16 |
        (uint32_t)slot << 11 |
        (uint32_t)func << 8 |
        (offset & 0xfc);

    outl(0xCF8, address); //write out the address
    return inl(0xCFC); //read in the data
}


const char *pci_device_class2string(uint32_t class, uint16_t subclass)
{
    switch (class) {
        case 1:
            switch (subclass) {
                case 1: return "IDE controller";
                default: break;
            }
            break;
        case 2:
            switch (subclass) {
                case 0: return "Ethernet controller";
                default: break;
            }
            break;
        case 3:
            switch (subclass) {
                case 0: return "VGA-Compatible controller";
                default: break;
            }
            break;
        case 6:
            switch (subclass) {
                case 0x00: return "Host bridge";
                case 0x01: return "ISA bridge";
                case 0x80: return "Other bridge service";
                default: break;
            }
            break;
        default:
            return NULL;
    }
}


uint8_t checkFunction(uint16_t bus, uint16_t device, uint8_t funct)
{
    pci_device_info i;
    pci_device_info_read(bus, device, funct, &i);

    if(i.vendor == 0xffff) return;

    printf("  %03i:%02i.%i ", bus, device, funct);
    char *s = NULL;
    s = pci_device_class2string(i.class, i.subclass);
    if(s == NULL) printf("class: %x subclass: %x ", i.class, i.subclass);
    else printf("%s: ", s);


    char *s2 = NULL;
    switch (i.vendor) {
        case 0x8086: s = "Intel corporation";
            switch(i.device) {
                case 0x1237: s2 = "i440fx (chipset)"; break;
                case 0x7000: s2 = "82371SB PIIX3 ISA"; break;
                case 0x7010: s2 = "82371SB PIIX3 IDE"; break;
                case 0x100e: s2 = "82540EM Gigabit Ethernet Controller"; break;
                default: s2 = NULL;
            }
            break;
        case 0x10ec: s = "Realtek";
            switch(i.device) {
                case 0x8139: s2 = "RTL8139 Fast Ethernet NIC"; break;
                default: s2 = NULL;
            }
            break;
        default: s = NULL; break;
    }

    if(s == NULL) printf("vendorID: %x ", i.vendor);
    else printf("%s ", s);
    if(s2 == NULL) printf("deviceID: %x ", i.device);
    else printf("%s ", s2);
    if(i.revision != 0) printf("(rev %i)\n", i.revision);
    else printf("\n");

    #warning TODO
//    rtl8139_init(bus, device, funct);

	return 0;
}

void pci_device_info_read(uint16_t bus, uint16_t device, uint16_t function, pci_device_info *i)
{
    uint32_t *a = i;
    #warning TODO check memset
    memset(a, 0, sizeof(pci_device_info)/sizeof(uint32_t));
    for(uint8_t x = 0; x < sizeof(pci_device_info)/sizeof(uint32_t); x++)
        a[x] = pci_config_read32(bus, device, function, x*sizeof(uint32_t));
}

#define pci_vendor_get(bus, dev, fce) pci_config_read16(bus, dev, fce, 0)

void pci_init()
{
    //check all buses
    for(uint16_t bus = 0; bus < 256; bus++)
	   for(uint16_t device = 0; device < 32; device++)
       {
           uint16_t function = 0;
           if(pci_vendor_get(bus, device, function) == 0xFFFF) continue; // Device doesn't exist


           pci_device_info i;
           pci_device_info_read(bus, device, function, &i);

           checkFunction(bus, device, function);
           if((i.header_type & 0x80) != 0) // It is a multi-function device, so check remaining functions
               for(function = 1; function < 8; function++)
                   if(pci_vendor_get(bus, device, function) != 0xFFFF)
                       checkFunction(bus, device, function);
       }
}
