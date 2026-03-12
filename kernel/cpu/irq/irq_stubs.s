.globl irq0
.globl irq1
.globl irq2
.globl irq3
.globl irq4
.globl irq5
.globl irq6
.globl irq7
.globl irq8
.globl irq9
.globl irq10
.globl irq11
.globl irq12
.globl irq13
.globl irq14
.globl irq15

.extern irq_handler

irq0:
    pushl $0          # dummy error code
    pushl $32         # interrupt number
    jmp irq_common

irq1:
    pushl $0
    pushl $33  
    jmp irq_common

irq2:
    pushl $0
    pushl $34
    jmp irq_common

irq3:
    pushl $0
    pushl $35
    jmp irq_common

irq4:
    pushl $0
    pushl $36
    jmp irq_common

irq5:
    pushl $0
    pushl $37
    jmp irq_common

irq6:
    pushl $0
    pushl $38
    jmp irq_common

irq7:
    pushl $0
    pushl $39
    jmp irq_common

irq8:
    pushl $0
    pushl $40
    jmp irq_common

irq9:
    pushl $0
    pushl $41
    jmp irq_common

irq10:
    pushl $0
    pushl $42
    jmp irq_common

irq11:
    pushl $0
    pushl $43
    jmp irq_common

irq12:
    pushl $0
    pushl $44
    jmp irq_common

irq13:
    pushl $0
    pushl $45
    jmp irq_common

irq14:
    pushl $0
    pushl $46
    jmp irq_common

irq15:
    pushl $0
    pushl $47
    jmp irq_common

irq_common:
    pusha

xor %eax, %eax
mov %ds, %ax
push %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp              # pass pointer to interrupt frame
    call irq_handler

    mov %eax, %esp         #  switch to new stack

    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa
    add $8, %esp
    iret


