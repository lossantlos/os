
#define ASM_FILE
#include "multiboot2.h"

.section .multiboot_header, "a", @progbits

.align MULTIBOOT_HEADER_ALIGN
multiboot_header_start:
.long MULTIBOOT2_HEADER_MAGIC
.long MULTIBOOT_ARCHITECTURE_I386
.long multiboot_header_end - multiboot_header_start
.long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + multiboot_header_end - multiboot_header_start)

info_request_tag_start:
.short MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
.short 0
.long info_request_tag_end - info_request_tag_start
.long MULTIBOOT_TAG_TYPE_MMAP
.align 8
.long MULTIBOOT_TAG_TYPE_ELF_SECTIONS
.align 8
info_request_tag_end:
.short MULTIBOOT_HEADER_TAG_END
.short 0
.long 8

multiboot_header_end:
