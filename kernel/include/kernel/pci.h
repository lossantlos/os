
#ifndef _PCI_H
#define _PCI_H

#include <stdint.h>

#define PCI_CONFIG_DATA         0xcfc
#define PCI_CONFIG_ADDRESS      0xcf8


#define PCI_BAR_UNDEF   0
#define PCI_BAR_IO      1
#define PCI_BAR_MEM32   2
#define PCI_BAR_MEM32   3

typedef struct {
    union {
        struct {uint8_t bus:8, device:5, funct:3;} __attribute__ ((packed));
        uint16_t data;
    };
} pci_addr_t;


struct pci_device_header_00h
{
    uint32_t    BAR0:32, //base address 0
                BAR1:32,
                BAR2:32,
                BAR3:32,
                BAR4:32,
                BAR5:32,
                cardbus_cis_pointer:32,
                subsystem_vendor:16, subsystem:16,
                expansion_rom_base_address:32,
                capabilities_ptr:8, reserved0:24,
                reserved1:32,
                interrupt_line:8, interrupt_pin:8, min_grant:8, max_latency:8;
} __attribute__ ((packed));

struct pci_device_header_01h
{
    uint32_t    BAR0:32, //base address 0
                BAR1:32,
                bus_primary:8, bus_secondary:8, bus_subordinate:8, secondary_latency_timer:8,
                io_base:8, io_limit:8, secondary_status:16,
                memory_base:16, memory_limit:16,
                memory_base_prefetchable:16, memory_limit_prefetchable:16,
                prefetchable_base_upper32:32,
                prefetchable_limit_upper32:32,
                io_base_upper16:16, io_limit_upper16:16,
                capability_ptr:8, reserved:23,
                expansion_rom_base_address:32,
                interrupt_line:8, interrupt_pin:8, bridge_control:16;
};

struct pci_device_header_02h
{
    uint32_t    cardbus_socket_exca_base_address:32,
                capabilities_list_offset:8, reserved:8, secondary_status:16,
                pci_bus_number:8, cardbus_bus_number:8, subordinate_bus_number:8, cardbus_latency_timer:8,
                mem_bar0:32, //Memory base address 0
                mem_limit0:32, //Memory limit 0
                mem_bar1:32,
                mem_limit1:32,
                io_base_address0:32,
                io_limit0:32,
                io_base_address1:32,
                io_limit1:32,
                interrupt_line:8, interrupt_pin:8, bridge_control:16,
                bubsyste_device_id:16, subsystem_vendor_id:16,
                card_legacy_mode_bar:32;
};

struct pci_device_info_struct {
    uint32_t    vendor:16, device:16,
                command:16, status:16,
                revision:8, prog_if:8, subclass:8, class:8,
                cache_line_size:8, latency_timer:8, header_type:8, BIST:8;
                union {
                    struct pci_device_header_00h pdh0h;
                    struct pci_device_header_01h pdh1h;
                    struct pci_device_header_02h pdh2h;
                };
} __attribute__((packed));

typedef struct pci_device_info_struct pci_device_info;


#endif
