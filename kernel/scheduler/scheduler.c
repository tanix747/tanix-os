#include "scheduler.h"
#include "../memory/heap.h"
#include "../cpu/gdt/gdt.h"


typedef struct task {
    uint32_t* stack;          // current saved ESP (context)
    uint32_t* kernel_stack;   // base of 4KB allocation
    uint32_t  kernel_stack_top; // top address (for TSS esp0)
    uint32_t* user_stack;         
    uint32_t  user_stack_top;
    struct task* next;
} task_t;

static task_t* current_task = 0;
static task_t* task_list = 0;

void scheduler_init()
{
    current_task = 0;
    task_list = 0;
}



void scheduler_add_task(void (*func)())
{
    task_t* task = (task_t*) kmalloc(sizeof(task_t));

    uint32_t* stack = (uint32_t*) kmalloc(4096);
    uint32_t stack_top = (uint32_t)stack + 4096;
    task->kernel_stack = stack;
    task->kernel_stack_top = stack_top;

    // ================================
    // Build FULL fake interrupt frame
    // Must match registers_t exactly
    // ================================

    // ---- iret frame (bottom) ----
    stack_top -= 4;
    *(uint32_t*)stack_top = 0x202;              // eflags (IF enabled)

    stack_top -= 4;
    *(uint32_t*)stack_top = 0x08;               // cs

    stack_top -= 4;
    *(uint32_t*)stack_top = (uint32_t)func;     // eip

    // ---- interrupt metadata ----
    stack_top -= 4;
    *(uint32_t*)stack_top = 0;                  // err_code

    stack_top -= 4;
    *(uint32_t*)stack_top = 32;                 // int_no (IRQ0)

    // ---- pusha registers (match struct order) ----
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // eax
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // ecx
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // edx
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // ebx
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // esp (dummy)
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // ebp
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // esi
    stack_top -= 4; *(uint32_t*)stack_top = 0;  // edi

    // ---- ds (first popped after switch) ----
    stack_top -= 4;
    *(uint32_t*)stack_top = 0x10;               // kernel data segment

    task->stack = (uint32_t*)stack_top;

    // ================================
    // Linked list logic
    // ================================
    if (!task_list)
    {
        task_list = task;
        current_task = task;
        task->next = task;
    }
    else
    {
        task_t* temp = task_list;
        while (temp->next != task_list)
            temp = temp->next;

        temp->next = task;
        task->next = task_list;
    }
}




extern void task_switch(uint32_t* old_esp, uint32_t new_esp);

uint32_t* schedule(registers_t* regs)
{
    if (!current_task)
        return (uint32_t*)regs;

    current_task->stack = (uint32_t*)regs;

    current_task = current_task->next;


    return current_task->stack;
}


void scheduler_start()
{
    if (!current_task)
        return;

    asm volatile(
        "mov %0, %%esp      \n"
        "pop %%eax          \n"  // pop ds
        "mov %%ax, %%ds     \n"
        "mov %%ax, %%es     \n"
        "mov %%ax, %%fs     \n"
        "mov %%ax, %%gs     \n"
        "popa               \n"
        "add $8, %%esp      \n"
        "iret               \n"
        :
        : "r"(current_task->stack)
        : "eax"
    );
}

