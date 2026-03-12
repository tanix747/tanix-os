#ifndef GDT_H
#define GDT_H
#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS   0x18
#define USER_DS   0x20

#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_init();

#endif
