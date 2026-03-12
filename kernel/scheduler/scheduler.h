#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<isr.h>

#include <stdint.h>
#include "isr.h"
void scheduler_init();
void scheduler_add_task(void (*func)());
uint32_t* schedule(registers_t* regs);
void scheduler_start();



#endif
