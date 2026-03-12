#ifndef IRQ_H
#define IRQ_H
#include <stdint.h>   

void irq_install();
void irq_install_handler(int irq, void (*handler)(void));
void pit_init(uint32_t frequency);


#endif
