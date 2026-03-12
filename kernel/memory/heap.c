#include "heap.h"
#include "pmm.h"
#include "paging/paging.h"

#define HEAP_START 0x500000

static uint32_t heap_current = HEAP_START;

void heap_init()
{
    heap_current = HEAP_START;
}

void* kmalloc(uint32_t size)
{
    // Align to 4KB pages
    uint32_t pages = (size + 0xFFF) / 0x1000;

    uint32_t addr = heap_current;

    for (uint32_t i = 0; i < pages; i++)
    {
        uint32_t frame = pmm_alloc_frame();
        map_page(heap_current, frame, PAGE_PRESENT | PAGE_RW);
        heap_current += 0x1000;
    }

    return (void*)addr;
}
