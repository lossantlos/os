# multibooot2

#.include "multiboot2.h"
.set MULTIBOOT_HEADER_ALIGN,         8
.set MULTIBOOT2_HEADER_MAGIC,        0xe85250d6
.set MULTIBOOT_ARCHITECTURE_I386,    0
.set MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST, 1
.set MULTIBOOT_TAG_TYPE_MMAP,        6
.set MULTIBOOT_HEADER_TAG_END,       0

.section .multiboot_header, "a", @progbits

.align MULTIBOOT_HEADER_ALIGN
multiboot_header_start:
.long MULTIBOOT2_HEADER_MAGIC
.long MULTIBOOT_ARCHITECTURE_I386
.long multiboot_header_end - multiboot_header_start
.long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + multiboot_header_end - multiboot_header_start)

mmap_tag_start:
.short MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
.short 0
.long mmap_tag_end - mmap_tag_start
.long MULTIBOOT_TAG_TYPE_MMAP
.align 8
mmap_tag_end:
.short MULTIBOOT_HEADER_TAG_END
.short 0
.long 8

multiboot_header_end:
