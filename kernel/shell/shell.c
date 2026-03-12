#include "shell.h"
#include "input.h"
#include "vfs.h"
#include "../fs/ramfs.h"
#include "../drivers/screen.h"
#include "screen.h"
#include "scheduler.h"

void sched_task1()
{
    int x = 0;
    while (1)
    {
  
        print_int(x++);
        print ("    ");
        for (volatile int i = 0; i < 10000000; i++);
    }
}

void sched_task2()
{
    char y = 'a';

    while (1)
    {
        print_char(y++);
        print ("    ");
        
        if (y == 'z' + 1)
            y = 'a';

        for (volatile int i = 0; i < 10000000; i++);
    }
}
int strcmp(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b)
            return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

int strncmp(const char* a, const char* b, int n)
{
    while (n && *a && *b)
    {
        if (*a != *b)
            return *a - *b;
        a++; b++; n--;
    }
    return 0;
}

char* strchr(char* s, char c)
{
    while (*s)
    {
        if (*s == c)
            return s;
        s++;
    }
    return 0;
}

static char line[256];

void shell_init()
{
    print("TanixOS:");
    print(vfs_current->name);   // or your current dir name field
    print("> ");
}

void shell_execute(char* cmd)
{
    if (strcmp(cmd, "ls") == 0)
        ramfs_ls();

    else if (strncmp(cmd, "mkdir ", 6) == 0)
        ramfs_mkdir(cmd + 6);

    else if (strncmp(cmd, "cd ", 3) == 0)
        ramfs_cd(cmd + 3);

    else if (strncmp(cmd, "cf ", 3) == 0)
        ramfs_cf(cmd + 3);

    else if (strncmp(cmd, "open ", 5) == 0)
        ramfs_open(cmd + 5);

    else if (strncmp(cmd, "rm ", 3) == 0)
        ramfs_rm(cmd + 3);
    else if (strcmp(cmd, "clear") == 0){
    clear_screen();
}


else if (strcmp(cmd, "sched") == 0)
{
    print("Starting scheduling demo...\n");
    scheduler_add_task(sched_task1);
    scheduler_add_task(sched_task2);
    scheduler_start();
}

    else if (strncmp(cmd, "write ", 6) == 0)
    {
        // format: write filename content
        char* space = strchr(cmd + 6, ' ');
        if (space)
        {
            *space = '\0';
            ramfs_write(cmd + 6, space + 1);
        }
    }
    
    else if (strcmp(cmd, "help") == 0)
{
    print("Filesystem Commands Implemented in Tanix OS (RAMFS):\n\n");

    print("1. mkdir <name>\n");
    print("   Function: ramfs_mkdir(const char* name)\n");
    print("   Purpose: Creates a directory inside current directory.\n\n");

    print("2. ls\n");
    print("   Function: ramfs_ls()\n");
    print("   Purpose: Lists all files and directories in current directory.\n\n");

    print("3. cd <name>\n");
    print("   Function: ramfs_cd(const char* name)\n");
    print("   Purpose: Changes current directory.\n");
    print("   Special case: \"cd ..\" moves to parent.\n\n");

    print("4. cf <name>\n");
    print("   Function: ramfs_cf(const char* name)\n");
    print("   Purpose: Creates a new file in current directory.\n\n");

    print("5. write <name> <content>\n");
    print("   Function: ramfs_write(const char* name, const char* content)\n");
    print("   Purpose: Writes content into file (overwrites).\n\n");

    print("6. open <name>\n");
    print("   Function: ramfs_open(const char* name)\n");
    print("   Purpose: Reads and prints file content.\n\n");

    print("7. rm <name>\n");
    print("   Function: ramfs_rm(const char* name)\n");
    print("   Purpose: Removes file or directory from current directory.\n");
}
    else
        print("Unknown command\n");
}

void shell_run()
{
    if (input_ready())
    {
        input_read(line);
        shell_execute(line);
        print("TanixOS:");
        print(vfs_current->name);   // or your current dir name field
        print("> ");
    }
}
