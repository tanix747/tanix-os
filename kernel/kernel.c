#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "pic.h"
#include "irq.h"
#include "keyboard.h"
#include "screen.h"
#include "paging.h"
#include "memory/pmm.h"
#include "memory/heap.h"
#include "scheduler.h"
#include "fs/ramfs.h"
#include "fs/vfs.h"
#include "shell/shell.h"



int cursor_position = 0;
void timer_callback()
{
    // nothing needed here
}


void kernel_main()
{
    gdt_init();
    idt_init();
    isr_install();
    pic_remap(32,40);
    irq_install();
    pit_init(100);
    keyboard_install();
    paging_init();
    pmm_init(128 * 1024 * 1024);
    heap_init();
    ramfs_init();
    scheduler_init();
    shell_init();

    asm volatile("sti");   // enable interrupts

    scheduler_start();

    while (1)
    {
        shell_run();
    }
}

