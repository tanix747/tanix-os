.section .multiboot
.align 4
.long 0x1BADB002
.long 0x0
.long -(0x1BADB002 + 0x0)

.section .bss
.align 16
stack_bottom:
.skip 16384          # 16 KB stack
stack_top:

.section .text
.global _start
.type _start, @function

_start:
    mov $stack_top, %esp   # Set stack pointer
    call kernel_main

hang:
    cli
    hlt
    jmp hang



