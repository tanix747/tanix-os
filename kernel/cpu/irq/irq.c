#include "irq.h"
#include "idt.h"
#include "isr.h"
#include <stdint.h>
#include "../scheduler/scheduler.h"

#define IRQ_COUNT 16

void (*irq_routines[IRQ_COUNT])(void) = { 0 };


extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void irq_install()
{
    idt_set_gate(32, (uint32_t)irq0,  0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1,  0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2,  0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3,  0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4,  0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5,  0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6,  0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7,  0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8,  0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9,  0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
}
#include <stdint.h>



static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void irq_install_handler(int irq, void (*handler)(void))
{
    if (irq < IRQ_COUNT)
        irq_routines[irq] = handler;
      
}

registers_t* irq_handler(registers_t* regs)
{
    int irq = regs->int_no - 32;

    // Call custom handler if installed
    if (irq < IRQ_COUNT && irq_routines[irq])
    {
        irq_routines[irq]();
    }

    // If timer interrupt → run scheduler
    if (irq == 0)
    {
        regs = (registers_t*)schedule(regs);
    }

    // --- ALWAYS send EOI ---
    if (regs->int_no >= 40)
        outb(0xA0, 0x20);   // slave PIC

    outb(0x20, 0x20);       // master PIC

    return regs;
}








void pit_init(uint32_t frequency)
{
    uint32_t divisor = 1193182 / frequency;

    asm volatile("cli");

    outb(0x43, 0x34);  // channel 0, lobyte/hibyte, mode 2

    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);

    asm volatile("sti");
}


