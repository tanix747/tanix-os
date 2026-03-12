# TanixOS

TanixOS is a 32-bit hobby operating system written in C and x86 Assembly to understand how operating systems work internally.

It implements core kernel subsystems such as memory management, scheduling, interrupts, and a simple command shell.

---

# Features

### Bootloader

* Custom bootloader written in Assembly
* Loads kernel from disk
* Switches CPU to **32-bit protected mode**

### Kernel

* Interrupt Descriptor Table (IDT)
* Basic interrupt handling
* VGA text mode screen driver
* Keyboard input driver

### Memory Management

* Paging enabled
* Virtual memory mapping

### Process Scheduling

* Basic round-robin scheduler
* Multiple tasks supported

### File System

* Simple **RAMFS**
* Files stored in memory

## Shell Commands

TanixOS includes a minimal command line shell.
The following commands are currently implemented:

| Command           | Description                                               |
| ----------------- | --------------------------------------------------------- |
| `ls`              | List files and directories in the current RAMFS directory |
| `mkdir <name>`    | Create a new directory                                    |
| `cd <name>` | Change the current directory                              |
| `cf <filename>`   | Create a new file                                         |
| `open <filename>` | Open and display a file                                   |
| `rm <name>`       | Remove a file or directory                                |
| `clear`           | Clear the screen                                          |
| `sched`           | Run scheduler demo with multiple tasks                    |

### Example Usage

```
> mkdir docs
> chngedir docs
> cf notes.txt
> ls
> open notes.txt
> rm notes.txt
```


# Architecture

```
+--------------------+
|      Shell         |
+--------------------+
|    System Calls    |
+--------------------+
|      Kernel        |
|  Scheduler | VM    |
+--------------------+
|   Drivers (kbd)    |
+--------------------+
|      Hardware      |
+--------------------+
```

---

# Build

Requirements:

* gcc
* nasm
* qemu
* make

Build OS:

```
make
```

Run OS:

```
make run
```


# Author

**Tanishq Abhange**

Built as a hobby project to learn **low-level systems programming and OS development**.
