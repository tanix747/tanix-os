#include "gdt.h"

struct gdt_entry gdt[6];
struct gdt_ptr gp;

extern void gdt_flush(uint32_t);

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access       = access;
}

void gdt_init()
{
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base  = (uint32_t)&gdt;

    // NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // Kernel Code Segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Kernel Data Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    // User Code Segment (Ring 3)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

// User Data Segment (Ring 3)
gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gp);
}
