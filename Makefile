CC = i686-elf-gcc

CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu99 \
         -Ikernel \
         -Ikernel/cpu/gdt \
         -Ikernel/cpu/idt \
         -Ikernel/cpu/isr \
         -Ikernel/cpu/irq \
         -Ikernel/cpu/pic \
         -Ikernel/drivers \
         -Ikernel/cpu/ports \
         -Ikernel/memory \
         -Ikernel/memory/paging \
         -Ikernel/scheduler \
         -Ikernel/fs \
         -Ikernel/shell

LDFLAGS = -T kernel/linker.ld -nostdlib -ffreestanding

OBJS = \
	kernel/boot/boot.o \
	kernel/kernel.o \
	kernel/cpu/gdt/gdt.o \
	kernel/cpu/gdt/gdt_flush.o \
	kernel/cpu/idt/idt.o \
	kernel/cpu/idt/idt_flush.o \
	kernel/cpu/isr/isr.o \
	kernel/cpu/isr/isr_stubs.o \
	kernel/cpu/pic/pic.o \
	kernel/cpu/irq/irq.o \
	kernel/cpu/irq/irq_stubs.o \
	kernel/drivers/keyboard.o \
	kernel/cpu/ports/ports.o \
	kernel/drivers/screen.o \
	kernel/memory/paging/paging.o \
	kernel/memory/paging/paging_asm.o \
	kernel/memory/pmm.o \
	kernel/memory/heap.o \
	kernel/scheduler/scheduler.o \
	kernel/fs/vfs.o \
	kernel/fs/ramfs.o \
	kernel/shell/input.o \
	kernel/shell/shell.o

all: TanixOS.iso

kernel.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o kernel.bin $(OBJS)

TanixOS.iso: kernel.bin
	cp kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o TanixOS.iso iso

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/%.o: kernel/%.s
	$(CC) -c $< -o $@

run: TanixOS.iso
	qemu-system-i386 -cdrom TanixOS.iso

clean:
	find kernel -name "*.o" -delete
	rm -f kernel.bin TanixOS.iso
