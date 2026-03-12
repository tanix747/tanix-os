#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2


void paging_init();
void flush_tlb();


#endif
