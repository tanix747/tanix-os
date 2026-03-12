#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void pmm_init(uint32_t mem_size);
uint32_t pmm_alloc_frame();
void pmm_free_frame(uint32_t frame);

#endif
