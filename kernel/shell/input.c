#include "input.h"

static char buffer[INPUT_SIZE];
static int index = 0;
static int ready = 0;

void input_add_char(char c)
{
    if (c == '\n')
    {
        buffer[index] = '\0';
        ready = 1;
        index = 0;
        return;
    }

    if (index < INPUT_SIZE - 1)
        buffer[index++] = c;
}

int input_ready()
{
    return ready;
}

void input_read(char* dest)
{
    if (!ready) return;

    int i = 0;
    while (buffer[i])
    {
        dest[i] = buffer[i];
        i++;
    }
    dest[i] = '\0';

    ready = 0;
}
