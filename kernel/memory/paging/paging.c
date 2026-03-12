#include "paging.h"
#include "memory/pmm.h"

extern void load_page_directory(uint32_t);
extern void enable_paging();

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void paging_init()
{
    // Clear directory
    for (int i = 0; i < 1024; i++)
        page_directory[i] = 0x00000002; // RW but not present

    // Identity map first 4MB
    for (int i = 0; i < 1024; i++)
    {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW ;
    }

    // Put page table into directory
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW ;

    // Load page directory
    load_page_directory((uint32_t)page_directory);

    // Enable paging
    enable_paging();
}

void map_page(uint32_t virtual, uint32_t physical, uint32_t flags)
{
    uint32_t dir_index = virtual >> 22;
    uint32_t table_index = (virtual >> 12) & 0x3FF;

    uint32_t* page_table;

    // If page table not present
    if (!(page_directory[dir_index] & PAGE_PRESENT))
    {
        uint32_t table_phys = pmm_alloc_frame();

        page_table = (uint32_t*) table_phys;

        // Clear new table
        for (int i = 0; i < 1024; i++)
            page_table[i] = 0;

        page_directory[dir_index] = table_phys | PAGE_PRESENT | PAGE_RW ;
    }
    else
    {
        page_table = (uint32_t*)(page_directory[dir_index] & 0xFFFFF000);
    }

    page_table[table_index] = physical | flags;

    flush_tlb();
}

